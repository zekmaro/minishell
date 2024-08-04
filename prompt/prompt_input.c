/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 19:40:20 by vvobis            #+#    #+#             */
/*   Updated: 2024/08/04 10:55:15 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_accepted_input(	t_prompt *prompt, \
								uint32_t cursor_position[2], \
								char **input, \
								char buffer[])
{
	bool	do_refresh;

	do_refresh = true;
	if (buffer[0] == ESC)
		do_refresh = handle_escape_sequence(prompt, &buffer[1], \
											input, cursor_position);
	else if (buffer[0] == '\t')
		handle_tab(input, prompt);
	else if (buffer[0] == DEL)
		handle_backspace(*input, cursor_position, \
				ft_strlen(*input));
	else if (buffer[0] == '\n')
	{
		ft_putstr_fd(prompt->prompt, 1);
		cursor_position[1] = prompt->prompt_length + 1;
		cursor_position[0]++;
		return ;
	}
	else
		handle_single_char_input(input, buffer, cursor_position, &do_refresh);
	if (do_refresh == true && g_signal_flag != 2)
		prompt_refresh_line(*input, prompt->prompt_length + 1, cursor_position);
}

static char	*handle_input(	t_prompt *prompt, \
							char *input, \
							uint32_t cursor_position[2], \
							const char *delimiter)
{
	char		buffer[100];
	uint32_t	delimiter_length;
	int64_t		bytes_read;
	uint32_t	cursor_position_base;

	delimiter_length = ft_strlen(delimiter);
	cursor_position_base = prompt->prompt_length;
	while (1)
	{
		ft_bzero(buffer, 100);
		bytes_read = ft_read(0, buffer, &input, 20);
		if (g_signal_flag == 1)
			return (ft_putstr_fd("^C\n", 1), NULL);
		if (bytes_read > 15)
			handle_rapid_input(buffer, cursor_position, input, \
								cursor_position_base);
		else if ((buffer[0] == *delimiter && delimiter_length == 1) \
				|| ft_strncmp(&buffer[0], delimiter, delimiter_length) == 0)
			break ;
		else if (bytes_read >= 1)
			handle_accepted_input(prompt, cursor_position, &input, buffer);
		if (g_signal_flag == 2)
			return (NULL);
	}
	return (input);
}

void	prompt_handle_history(t_prompt *prompt, char *input)
{
	if (!input)
		return ;
	prompt->command = input;
	prompt->history_entries[prompt->history_count] = prompt->command;
	prompt->history_count++;
	prompt->history_position_current = prompt->history_count;
}

char	*prompt_get_input(	t_prompt *prompt, \
							uint32_t prompt_initial_size, \
							const char *delimiter)
{
	char		*input;

	input = ft_calloc(prompt_initial_size, sizeof(*input));
	if (!input)
		return (perror("malloc"), NULL);
	lst_memory(input, free, ADD);
	prompt->prompt_length = prompt_display_string_set(prompt, \
							(const char **)prompt->env_ptr, NULL);
	prompt->prompt_display_func(prompt->prompt);
	terminal_raw_mode_enable(ECHOCTL | ICANON);
	cursor_position_get(prompt->cursor_position);
	prompt->cursor_position[1] = 0;
	if (!delimiter)
		delimiter = "\n";
	input = handle_input(prompt, input, prompt->cursor_position, delimiter);
	terminal_raw_mode_disable(ECHO | ECHOCTL | ICANON);
	if (!input || !*input)
		return (NULL);
	prompt_handle_history(prompt, input);
	return (input);
}
