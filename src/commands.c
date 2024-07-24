/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 21:20:49 by victor            #+#    #+#             */
/*   Updated: 2024/07/24 12:25:59 by anarama          ###   ########.fr       */
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

bool	buildin_execute(char const *command, char const **argv)
{
	if (ft_strncmp(command, "exit\0", 5) == 0)
		ft_exit();
	else if (ft_strncmp(command, "cd\0", 3) == 0)
	{
		ft_chdir(argv[1]);
		return (true);
	}
	// if (ft_strncmp(command, "echo", ft_strlen(command)) == 0)
	// {
	// 	ft_echo(argv, STDOUT_FILENO);
	// }
	// else if (ft_strncmp(tokens[0], "pwd", ft_strlen(tokens[0])) == 0)
	// {
	// 	ft_pwd(STDOUT_FILENO, env);
	// }
	// else if (ft_strncmp(tokens[0], "env", ft_strlen(tokens[0])) == 0)
	// {
	// 	ft_env(env, STDOUT_FILENO);
	// }
	return (false);
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

void execute_commands(t_ast *ast)
{
    t_ast		*current = ast;
	static int	exit_status;

	exit_status = 0;
    while (current)
    {
		if (current->type == NODE_COMMAND && !current->is_done)
        {
			if (!buildin_execute(current->args[0], current->args))
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
	execute_commands(ast);
	restore_fd(original_stdin, original_stdout);
	//print_ast(ast);
	return (NULL);
}
