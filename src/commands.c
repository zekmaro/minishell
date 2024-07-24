/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 21:20:49 by victor            #+#    #+#             */
/*   Updated: 2024/07/24 17:04:37 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <fcntl.h>

void	restore_fd(int original_stdin, int original_stdout)
{
	dup2(original_stdin, STDIN_FILENO);
	dup2(original_stdout, STDOUT_FILENO);
	close(original_stdin);
	close(original_stdout);
}

bool	buildin_execute(t_ast *node, const char **environment)
{
	if (ft_strncmp(node->args[0], "echo", ft_strlen(node->args[0])) == 0)
		return (ft_echo(node), 1);
	else if (ft_strncmp(node->args[0], "env", ft_strlen(node->args[0])) == 0)
		return (ft_env(environment), 1);
	else if (ft_strncmp(node->args[0], "cd", ft_strlen(node->args[0])) == 0)
		return (ft_cd(environment, (const char **)node->args), 1);
	else if (ft_strncmp(node->args[0], "unset", ft_strlen(node->args[0])) == 0)
		return (ft_unset((char **)environment, (const char **)node->args), 1);
	else if (ft_strncmp(node->args[0], "export", ft_strlen(node->args[0])) == 0)
		return (ft_export((char ***)&environment, (const char **)node->args), 1);
	else if (ft_strncmp(node->args[0], "exit", ft_strlen(node->args[0])) == 0)
		return (lst_memory(NULL, NULL, END), 1);
	return (0);
}

void	command_execute(char const *command_path,
						char const *argv[],
						char const **env)
{
	pid_t	child_process;

	ft_fork(&child_process, command_path);
	if (child_process == 0)
	{
		execve(command_path, (char **)argv, (char **)env);
		perror("execve");
		lst_memory(NULL, NULL, CLEAN);
	}
	else
	{
		waitpid(child_process, NULL, 0);
	}
}

int execute_command(t_ast *command)
{
	int		status;
    pid_t	pid;
	int		fd;
	
	pid = fork();
	fd = 0;
    if (pid == -1)
    {
        perror("fork");
        lst_memory(NULL, NULL, CLEAN);
    }
    else if (pid == 0)
    {
        if (command->fd_in != 0)
        {
            dup2(command->fd_in, 0);
            close(command->fd_in);
        }
        if (command->fd_out != 1)
        {
            dup2(command->fd_out, 1);
            close(command->fd_out);
        }
		if (command->file)
        {
			ft_open(&fd, command->file, command->flags, 0644);
            if (command->std_fd == STDIN_FILENO)
            {
                dup2(fd, STDIN_FILENO);
                close(fd);
            }
            else if (command->std_fd == STDOUT_FILENO)
            {
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }
        }
        execvp(command->args[0], command->args);
        perror("execvp");
        exit(EXIT_FAILURE);
    }
    else
    {
        if (command->fd_in != 0)
        {
            close(command->fd_in);
        }
        if (command->fd_out != 1)
        {
            close(command->fd_out);
        }
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
        {
            return WEXITSTATUS(status);
        }
        else
        {
            return (-1);
        }
    }
}

void execute_commands(t_ast *ast, const char **environment)
{
    t_ast		*current = ast;
	static int	exit_status;

	exit_status = 0;
    while (current)
    {
		if (current->type == NODE_COMMAND && !current->is_done)
        {
			if (!buildin_execute(current, environment))
            	exit_status = execute_command(current);
        }
		else if (current->type == NODE_LOGICAL_OPERATOR)
		{
			if (current->token_type == TOKEN_AND)
			{
				if (exit_status == 1)
				{
					if (current->right)
						current = current->right->right;
					continue ;
				}
			}
			else if (current->token_type == TOKEN_OR)
			{
				if (exit_status == 0)
				{
					if (current->right)
						current = current->right->right;
					continue ;
				}
			}
		}
        current = current->right;
    }
}

void	traverse_tree(t_ast	*ast, t_ast **head)
{
	while (ast)
	{
		if (ast->type == NODE_REDIRECTION)
		{
			handle_redir(ast, head);
		}
		else if (ast->type == NODE_PIPE)
		{
			handle_pipe(ast);
		}
		ast = ast->right;
	}
}

void	*m_tokenizer(const char *input, const char **env, const char *path_variable)
{
	t_token	*tokens;
	t_ast	*ast;
	int	original_stdin = dup(STDIN_FILENO);
	int	original_stdout = dup(STDOUT_FILENO);

	lst_memory((void *)input, free, ADD);
	(void)path_variable;
	tokens = lexical_analysis(input, env);
	//print_tokens(tokens);
	ast = parse_tokens(tokens);
	//print_ast(ast);
	traverse_tree(ast, &ast);
	//print_ast(ast);
	execute_commands(ast, env);
	restore_fd(original_stdin, original_stdout);
	//print_ast(ast);
	return (NULL);
}
