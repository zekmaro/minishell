/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arrowkeys.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 11:02:39 by victor            #+#    #+#             */
/*   Updated: 2024/07/20 18:48:41 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdint.h>

static void	handle_arrow_key_up(	t_prompt *prompt,
									char **input,
									uint32_t *cursor_position_current)
{
	uint32_t	prompt_history_count;

	if (prompt->history_count == 0)
		return ;
	prompt_history_count = prompt->history_count;
	if (prompt->history_position_current == prompt->history_count && \
			prompt->history_entries[prompt_history_count] == 0)
		prompt->history_entries[prompt_history_count] = *input;
	*input = prompt->history_entries[--prompt->history_position_current];
	*cursor_position_current = ft_strlen(*input);
}

static void	handle_arrow_key_down(	t_prompt *prompt,
									char **input,
									uint32_t *cursor_position_current)
{
	*input = prompt->history_entries[++prompt->history_position_current];
	*cursor_position_current = ft_strlen(*input);
}

static void	handle_arrow_key_right(	uint32_t *cursor_position_current,
									uint32_t prompt_length_current)
{
	if (*cursor_position_current >= prompt_length_current)
		return ;
	(*cursor_position_current)++;
	ft_printf(CURSOR_MOVE_RIGHT);
}

static void	handle_arrow_key_left(	uint32_t *cursor_position_current)
{
	if (*cursor_position_current <= 0)
		return ;
	(*cursor_position_current)--;
	ft_printf(CURSOR_MOVE_LEFT);
}

uint8_t	handle_escape_sequence(	t_prompt *prompt,
											char	**input,
											uint32_t *cursor_position_current)
{
	char		escape_sequence[2];
	uint32_t	prompt_length_current;

	prompt_length_current = ft_strlen(*input);
	ft_read(0, escape_sequence, input, 2);
	if (escape_sequence[0] == 91 && escape_sequence[1] == 65
		&& prompt->history_position_current > 0)
		handle_arrow_key_up(prompt, input, cursor_position_current);
	else if (escape_sequence[0] == 91 && escape_sequence[1] == 66
		&& prompt->history_position_current < prompt->history_count)
		handle_arrow_key_down(prompt, input, cursor_position_current);
	else if (escape_sequence[0] == 91 && escape_sequence[1] == 67)
		handle_arrow_key_right(cursor_position_current, prompt_length_current);
	else if (escape_sequence[0] == 91 && escape_sequence[1] == 68)
		handle_arrow_key_left(cursor_position_current);
	return (1);
}
