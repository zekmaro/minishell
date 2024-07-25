/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 21:20:49 by victor            #+#    #+#             */
/*   Updated: 2024/07/25 12:37:39 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
	return (false);
}

void	command_execute(const char *command_path,
						const char **argv,
						const char **env)
{
	pid_t	child_proccess;

	ft_fork(&child_proccess, command_path);
	if (child_proccess == 0)
	{
		execve(command_path, (char **)argv, (char **)env);
		perror("execve");
		lst_memory(NULL, NULL, CLEAN);
	}
	else
	{
		waitpid(child_proccess, NULL, 0);
	}
}

void	redirect_fd_into_file(t_ast *command)
{
	int	fd;

	fd = 0;
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

void	handle_fds_child_proccess(t_ast *command)
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
		redirect_fd_into_file(command);
	}
}

void	handle_fds_parent_proccess(t_ast *command)
{
	if (command->fd_in != 0)
	{
		close(command->fd_in);
	}
	if (command->fd_out != 1)
	{
		close(command->fd_out);
	}
 	if (command->file)
	{
		redirect_fd_into_file(command);
	}
}

int	execute_command(t_ast *command, const char **env)
{
	int		status;
	pid_t	pid;

	ft_fork(&pid, "execute command");
	if (pid == 0)
	{
		handle_fds_child_proccess(command);
		execve(command->path, command->args, (char **)env);
		perror("execve");
		exit(EXIT_FAILURE);
	}
	else
	{
		handle_fds_parent_proccess(command);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
		{
			return (WEXITSTATUS(status));
		}
	}
	return (-1);
}

void	execute_commands(t_ast *ast, const char *path_variable,
					const char **env)
{
	static int	exit_status;
	t_ast		*current;

	exit_status = 0;
	current = ast;
	while (current)
	{
		if (current->type == NODE_COMMAND && !current->is_done)
		{
			if (!buildin_execute(current->args[0],
					(const char **)current->args))
			{
				current->path = find_absolute_path(path_variable,
						current->args[0]);
				exit_status = execute_command(current, env);
			}
		}
		else if (current->type == NODE_LOGICAL_OPERATOR)
		{
			handle_logical_operator(&current, exit_status);
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

void	print_tokens(t_token *tokens)
{
	int i;

	i = 0;
	printf("----TOKENS----\n");
	while (tokens[i].token_type != TOKEN_EOL)
	{
		printf("Token: Type=%d, Value=%s\n", tokens[i].token_type, tokens[i].token_value);
		i++;
	}
	printf("------------\n");
}

void	*m_tokenizer(const char *input, const char **env,
			const char *path_variable)
{
	t_token	*tokens;
	t_ast	*ast;
	int		original_stdin;
	int		original_stdout;

	original_stdin = dup(STDIN_FILENO);
	original_stdout = dup(STDOUT_FILENO);
	lst_memory((void *)input, free, ADD);
	tokens = lexical_analysis(input, env);
	print_tokens(tokens);
	ast = parse_tokens(tokens);
	print_ast(ast);
	traverse_tree(ast, &ast);
	print_ast(ast);
	execute_commands(ast, path_variable, env);
	restore_fd(original_stdin, original_stdout);
	return (NULL);
}
