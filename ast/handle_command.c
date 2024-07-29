/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 18:14:10 by anarama           #+#    #+#             */
/*   Updated: 2024/07/29 23:43:04 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_pipe_in_child(t_ast *command)
{
	ft_dup2(command->pipefd[1], STDOUT_FILENO, "dup2 in pipe_child");
	ft_close(command->pipefd[0], "close in pipe_child");
	ft_close(command->pipefd[1], "close in pipe_child");
}

void	handle_pipe_in_parent(t_ast *command)
{
	ft_close(command->pipefd[1], "close in pipe_parent");
	ft_dup2(command->pipefd[0], STDIN_FILENO, "dup2 in pipe_parent");
	ft_close(command->pipefd[0], "close in pipe_parent");
}

int	execute_command(t_ast *command, const char **env)
{
	int		status;
	int		stdout_fd;
	pid_t	pid;

	stdout_fd = dup(STDOUT_FILENO);
	ft_fork(&pid, "execute command");
	if (pid == 0)
	{
		if (command->connection_type == TREE_PIPE)
			handle_pipe_in_child(command);
		if (command->has_redir_in || command->has_redir_out)
			handle_fds_child_proccess(command);
		if (buildin_execute(command, (const char **)env))
			lst_memory(NULL, NULL, END);
		execve(command->path, command->args, (char **)env);
		perror("execve");
		lst_memory(NULL, NULL, CLEAN);
	}
	else
	{
		if (command->connection_type == TREE_PIPE)
			handle_pipe_in_parent(command);
		if (command->has_redir_in || command->has_redir_out)
			handle_fds_parent_proccess(command);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
		{
			dup2(stdout_fd, STDOUT_FILENO);
			return (WEXITSTATUS(status));
		}
	}
	return (-1);
}

void	handle_command(t_ast *current, const char *path_variable,
					const char **env, int *exit_status)
{

	if (current->connection_type == TREE_PIPE)
	{
		ft_pipe(current->pipefd, "in handle_command");
	}
	current->path = find_absolute_path(path_variable,
			current->args[0]);
	*exit_status = execute_command(current, env);
}
