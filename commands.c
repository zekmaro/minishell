/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 21:20:49 by victor            #+#    #+#             */
/*   Updated: 2024/07/16 19:49:14 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

// void	*m_tokenizer(const char *input, const char **env, const char *path_variable)
// {
// 	char	**tokens;
// 	char	*command_path;
// 	int		original_stdin = dup(STDIN_FILENO);
// 	int		original_stdout = dup(STDOUT_FILENO);

// 	tokens = ft_split(input, ' ');
// 	lst_memory(tokens, &free_split, ADD);
// 	if (!*tokens)
// 		return (NULL);
// 	execute(tokens, (char **)env);
// 	if (buildin_execute(tokens[0], (char const **)tokens))
// 	{
// 		restore_fd(original_stdin, original_stdout);
// 		return (NULL);
// 	}
// 	command_path = find_absolute_path(path_variable, (char *)*tokens);
// 	if (command_path == 0)
// 	{
// 		restore_fd(original_stdin, original_stdout);
// 		return (NULL);
// 	}
// 	command_execute(command_path, (char const **)tokens, env);
// 	restore_fd(original_stdin, original_stdout);
// 	free_split(tokens);
// 	return (NULL);
// }
