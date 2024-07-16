/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 19:40:20 by vvobis            #+#    #+#             */
/*   Updated: 2024/07/16 11:01:37 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"
#include <stdint.h>

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
}

static void	handle_new_character_to_input(	char **input,
											char character,
											uint32_t *cursor_position_current,
											uint32_t prompt_length_current)
{
	if (*cursor_position_current < prompt_length_current)
		ft_memmove(&(*input)[*cursor_position_current + 1], \
					&(*input)[*cursor_position_current], \
					prompt_length_current - *cursor_position_current);
	(*input)[*cursor_position_current] = character;
	(*cursor_position_current)++;
}

static uint8_t handle_single_char_input(char **input, char character, \
										uint32_t cursor_position_current[2])
{
	uint32_t	input_length_current;

	input_length_current = ft_strlen(*input);
	if (ft_isprint(character))
		handle_new_character_to_input(	input, character, \
										&cursor_position_current[1], \
										input_length_current);
	else if (character == DEL)
		handle_backspace(*input, &cursor_position_current[1], input_length_current);
	else if (character == '\n')
		return (ft_putstr_fd("\n", 1), ft_putstr_fd(SCREEN_CLEAR_TO_EOF, 1), 1);
	else if (character == 4 && input_length_current == 0)
		return (ft_putstr_fd("\n", 1), lst_memory(NULL, NULL, CLEAN), 0);
	return (0);
}

static void	handle_input(	t_prompt *prompt, \
							char *input, \
							uint32_t cursor_position_base)
{
	uint32_t	cursor_position_current[2];
	char		character; 

	cursor_position_get(cursor_position_current);
	cursor_position_current[1] = 0;
	while (1)
	{
		ft_read(0, &character, &input, 1);
		if (g_signal_flag == 1)
			return (lst_memory(input, NULL, FREE));
		if (	(ft_isprint(character) || character == DEL \
				|| character == '\n' || character == EOT) \
				&& handle_single_char_input(&input, character, \
											cursor_position_current))
				break ;
		else
		{
			if (character == ESC)
				handle_escape_sequence(prompt, &input, &cursor_position_current[1]);
			else if (character == '\t')
				handle_tab(	&input, (const char **)prompt->env_ptr, \
							&cursor_position_current[1]);
			cursor_position_restore();
		}
		prompt_refresh_line(input, (uint32_t [2]){	cursor_position_current[0], \
													cursor_position_current[1] \
													+ cursor_position_base});
	}
	prompt->command = input;
}

char	*prompt_get_input(t_prompt *prompt)
{
	char		*input;
	uint32_t	cursor_position_base;

	cursor_position_base = prompt->prompt_length + 1;
	input = ft_calloc(PROMPT_INPUT_BUFFER_SIZE, sizeof(*input));
	lst_memory(input, free, ADD);
	cursor_position_save();
	handle_input(prompt, input, cursor_position_base);
	return (input);
}
