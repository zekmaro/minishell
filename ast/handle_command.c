/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 18:14:10 by anarama           #+#    #+#             */
/*   Updated: 2024/07/30 12:42:14 by victor           ###   ########.fr       */
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
	pid_t	pid;

	ft_fork(&pid, "execute command");
	if (pid == 0)
	{
		if (command->connection_type == TREE_PIPE)
			handle_pipe_in_child(command);
		if (command->has_redir_in || command->has_redir_out)
			handle_fds_child_proccess(command);
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
			return (WEXITSTATUS(status));
		}
	}
	return (-1);
}

void	buildin_apply_pipe(t_ast *node)
{
	if (node->connection_type == TREE_PIPE)
	{
		ft_dup2(node->pipefd[1], STDOUT_FILENO, "dup2 in buildin_execute");
		ft_dup2(node->pipefd[0], STDIN_FILENO, "dup2 in buildin_execute");
		ft_close(node->pipefd[0], "close in buildin_execute");
		ft_close(node->pipefd[1], "close in buildin_execute");
	}
}

bool	buildin_execute(t_ast *node, const char **environment, int *exit_status)
{
	if (node->args[0] && !*node->args[0])
		return (false);
	if (ft_strncmp(node->args[0], "echo", ft_strlen(node->args[0])) == 0)
		return (buildin_apply_pipe(node), ft_echo(node->args), 1);
	else if (ft_strncmp(node->args[0], "env", ft_strlen(node->args[0])) == 0)
		return (buildin_apply_pipe(node), ft_env(environment), 1);
	else if (ft_strncmp(node->args[0], "cd", ft_strlen(node->args[0])) == 0)
		return (buildin_apply_pipe(node), ft_cd(environment, (const char **)node->args), 1);
	else if (ft_strncmp(node->args[0], "unset", ft_strlen(node->args[0])) == 0)
		return (buildin_apply_pipe(node), ft_unset((char **)environment, (const char **)node->args), 1);
	else if (ft_strncmp(node->args[0], "export", ft_strlen(node->args[0])) == 0)
		return (buildin_apply_pipe(node), ft_export((char ***)&environment, (const char **)node->args), 1);
	else if (ft_strncmp(node->args[0], "exit", ft_strlen(node->args[0])) == 0)
		return (lst_memory(NULL, NULL, END), 1);
	return (0);
}

void	handle_command(t_ast *current, const char *path_variable,
					const char **env, int *exit_status)
{
	int		stdout_fd;

	stdout_fd = dup(STDOUT_FILENO);
	if (current->connection_type == TREE_PIPE)
	{
		ft_pipe(current->pipefd, "in handle_command");
	}
	if (!buildin_execute(current, env, exit_status))
	{
		current->path = find_absolute_path(path_variable,
				current->args[0]);
		*exit_status = execute_command(current, env);
	}
	dup2(stdout_fd, STDOUT_FILENO);
}
