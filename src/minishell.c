/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 12:22:34 by victor            #+#    #+#             */
/*   Updated: 2024/07/24 15:14:29 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int32_t	g_signal_flag;

void	setup(char **path_variable)
{
	g_signal_flag = 0;
	*path_variable = getenv("PATH");
	if (!path_variable)
		return (p_stderr(2, \
				"No Valid Path Variable was Found in Environment.\nExiting\n", \
				NULL), exit (1));
	setup_signal_handlers();
	ft_printf(SCREEN_CLEAR);
	ft_printf(CURSOR_MOVE_HOME);
}

int	main(int argc, char **argv, const char **env)
{
	t_prompt	*prompt;
	char		*path_variable;
	char		*command_input;
	char		**environment;

	(void)argv;
	(void)argc;
	setup(&path_variable);
	environment = environment_create(env);
	prompt = prompt_create((const char **)environment);
	while (1)
	{
		command_input = prompt_get(prompt);
		if (g_signal_flag == 1 || !command_input)
		{
			if (g_signal_flag == 1)
				g_signal_flag = 0;
			continue ;
		}
		if (command_input && *command_input != '\n')
		{
			m_tokenizer(command_input, (const char **)environment, path_variable);
			prompt->history_entries[prompt->history_count++] = prompt->command;
		}
		prompt->history_position_current = prompt->history_count;
		lst_memory(command_input, NULL, FREE);
	}
}
