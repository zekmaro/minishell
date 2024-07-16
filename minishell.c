/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 12:22:34 by victor            #+#    #+#             */
/*   Updated: 2024/07/16 10:26:59 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int32_t	g_signal_flag;

void	setup(char **path_variable)
{
	g_signal_flag = 0;
	*path_variable = getenv("PATH");
	if (!path_variable)
		return (p_stderr(2, \
				"No Valid Path Variable was Found in Environment.\nExiting\n", \
				NULL), exit (1));
	terminal_raw_mode_enable();
	setup_signal_handlers();
	ft_printf(SCREEN_CLEAR);
	ft_printf(CURSOR_MOVE_HOME);
}

int	main(int argc, char **argv, char **env)
{
	t_prompt	*prompt;
	char		*path_variable;

	(void)argv;
	(void)argc;
	setup(&path_variable);
	prompt = prompt_create((const char **)env);
	while (1)
	{
		prompt_get(prompt);
		if (g_signal_flag == 1 || !*prompt->command)
		{
			if (g_signal_flag == 1)
			{
				ft_putstr_fd(SCREEN_CLEAR_TO_EOF, 1);
				ft_putchar_fd('\n', 1);
			}
			g_signal_flag = 0;
			continue ;
		}
		if (*prompt->command && *prompt->command != '\n')
		{
			m_tokenizer(prompt->command, (const char **)env, path_variable);
			prompt->history_entries[prompt->history_count++] = prompt->command;
		}
		prompt->history_position_current = prompt->history_count;
	}
	terminal_raw_mode_disable();
}
