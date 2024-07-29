/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 19:40:20 by vvobis            #+#    #+#             */
/*   Updated: 2024/07/28 15:58:45 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <sys/ioctl.h>
#define nop() {volatile int x; {x = 0;}}

static void	handle_backspace(	char *input, \
								uint32_t *cursor_position_current, \
								uint32_t input_length_current)
{
	if (*cursor_position_current <= 0)
		return ;
	(*cursor_position_current)--;
	ft_memmove(&input[*cursor_position_current], \
				&input[*cursor_position_current + 1], \
				input_length_current - *cursor_position_current);
	ft_putstr_fd(CURSOR_MOVE_LEFT, 1);
}

static bool	handle_new_character_to_input(	char **input,
											char character,
											uint32_t *cursor_position_current,
											uint32_t prompt_length_current)
{
	bool	do_refresh;

	do_refresh = false;
	prompt_buffer_size_manage(input, prompt_length_current, ft_strlen(*input) * 2);
	if (*cursor_position_current < prompt_length_current)
	{
		ft_memmove(&(*input)[*cursor_position_current + 1], \
				&(*input)[*cursor_position_current], \
				prompt_length_current - *cursor_position_current);
		do_refresh = true;
	}
	(*input)[*cursor_position_current] = character;
	(*cursor_position_current)++;
	return (do_refresh);
}

bool	handle_multiple_character_to_input(	char **input,
												char buffer[],
												uint32_t *cursor_position_current,
												uint32_t prompt_length_current)
{
	bool		do_refresh;
	uint32_t	buffer_length;

	do_refresh = false;
	buffer_length = ft_strlen(buffer);
	*input = prompt_buffer_size_manage(input, prompt_length_current, buffer_length + 1);
	if (*cursor_position_current < prompt_length_current - 1)
	{
		ft_memmove(&(*input)[*cursor_position_current + buffer_length], \
				&(*input)[*cursor_position_current], \
				prompt_length_current - *cursor_position_current + buffer_length);
		do_refresh = true;
	}
	ft_memcpy(&(*input)[*cursor_position_current], buffer, buffer_length);
	(*cursor_position_current) += buffer_length;
	return (do_refresh);
}

static uint8_t	handle_single_char_input(char **input, char buffer[], \
										uint32_t cursor_position_current[2], bool *do_refresh)
{
	uint32_t	input_length_current;
	uint32_t	new_buffer_length;
	uint32_t	i;

	input_length_current = ft_strlen(*input);
	new_buffer_length = ft_strlen(buffer);
	if (new_buffer_length == 1)
	{
		if (ft_isprint(buffer[0]))
			return (*do_refresh = handle_new_character_to_input(input, buffer[0], &cursor_position_current[1], input_length_current), 1);
		else if (buffer[0] == EOT && input_length_current == 0)
			return (ft_putstr_fd("\n", 1), terminal_raw_mode_disable(ECHOCTL), ft_free(&*input), lst_memory(NULL, NULL, CLEAN), 1);
	}
	else
	{
		i = 0;
		*do_refresh = handle_multiple_character_to_input(input, buffer, &cursor_position_current[1], input_length_current);
		return (1);
	}
	return (0);
}

void	blocking_mode_toggle(int flag)
{
	if (ioctl(0, FIONBIO, &flag) == -1)
	{
		perror("ioctl");
		exit(1);
	}
}

void	your_sleep(int ms)
{
	volatile int i;

	i = 1000000 * ms;
	while (i--)
		nop();
}

static char	*handle_input(	t_prompt *prompt, \
							char *input, \
							uint32_t cursor_position_base)
{
	uint32_t	cursor_position[2];
	char		buffer[100];
	bool		do_refresh;
	bool		ignore_newline;
	int64_t		bytes_read;

	cursor_position_get(cursor_position);
	cursor_position[1] = 0;
	ignore_newline = false;
	while (1)
	{	
		ft_bzero(buffer, 100);
		bytes_read = ft_read(0, buffer, &input, 20);
		if (g_signal_flag == 1)
				
			return (input[cursor_position[1]] = '^', input[cursor_position[1] + 1] = 'C', prompt_refresh_line(input, cursor_position_base, cursor_position), ft_putstr_fd("\n", 1), ft_putstr_fd(SCREEN_CLEAR_TO_EOF, 1), NULL);
		if (bytes_read > 0)
		{
			do_refresh = true;
			if (bytes_read > 4 && ignore_newline == false)
			{
				blocking_mode_toggle(1);
				while (bytes_read > 0)
				{
					handle_single_char_input(&input, buffer, cursor_position, &do_refresh);
					ft_bzero(buffer, 100);
					bytes_read = ft_read(0, buffer, &input, 99);
				}
				blocking_mode_toggle(0);
			}
			else if (bytes_read >= 1)
			{
				if (buffer[0] == ESC)
					do_refresh = handle_escape_sequence(prompt, &buffer[1], &input, cursor_position);
				else if (buffer[0] == '\t')
					handle_tab(&input, (const char **)prompt->env_ptr, \
							&cursor_position[1]);
				else if (buffer[0] == DEL)
					handle_backspace(input, &cursor_position[1], \
							ft_strlen(input));
				else
					handle_single_char_input(&input, buffer, cursor_position, &do_refresh);
			}
			if (ignore_newline == false && buffer[0] == '\n')
				break;
			if (do_refresh == true)
				prompt_refresh_line(input, cursor_position_base, cursor_position);
		}
	}
	prompt->command = input;
	return (input);
}

char	*prompt_get_input(t_prompt *prompt)
{
	char		*input;
	char		*new_line_terminator;
	uint32_t	cursor_position_base;

	cursor_position_base = prompt->prompt_length + 1;
	input = ft_calloc(PROMPT_INPUT_BUFFER_SIZE, sizeof(*input));
	terminal_raw_mode_enable(ECHOCTL | ICANON);
	handle_input(prompt, input, cursor_position_base);
	terminal_raw_mode_disable(ECHOCTL | ICANON);
	return (prompt->command);
}
