/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 21:20:49 by victor            #+#    #+#             */
/*   Updated: 2024/07/21 17:00:55 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

void	*m_tokenizer(const char *input, const char **env, const char *path_variable)
{
	t_token	**tokens;
	t_ast	*ast;

	ft_printf("%s\n", input);
	tokens = lexical_analysis(input, env);
	printf("---TOKENS---\n");
	print_tokens(tokens);
	printf("------------\n");
	ast = parse_tokens(tokens);
	printf("----AST----\n");
	print_ast(ast);
	printf("-----------\n");
	return (NULL);
}


