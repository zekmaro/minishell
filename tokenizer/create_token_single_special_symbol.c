/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_token_single_special_symbol.c               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 12:36:57 by anarama           #+#    #+#             */
/*   Updated: 2024/07/28 19:45:39 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_single_special(const char input)
{
	return ((input == '<')
		|| (input == '>')
		|| (input == '|'));
}

t_token	create_token_single_special_symbol(const char **input)
{
	t_token		temp_token;
	uint32_t	i;
	char		*temp_move;

	i = 0;
	temp_move = (char *)*input;
	if (**input == '<')
		temp_token = create_token(TOKEN_REDIRECT_IN, &(*input)[i]);
	else if (**input == '>')
		temp_token = create_token(TOKEN_REDIRECT_OUT, &(*input)[i]);
	else
		temp_token = create_token(TOKEN_PIPE, &(*input)[i]);
	if (temp_move)
	{
		*temp_move = 0;
		*input = temp_move + 1;
	}
	else
	{
		temp_move = (char *)*input;
		*input += ft_strlen(*input);
		*temp_move = 0;
	}
	return (temp_token);
}
