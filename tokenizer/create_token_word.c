/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_token_word.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 12:40:09 by anarama           #+#    #+#             */
/*   Updated: 2024/07/22 13:45:48 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	create_token_word(const char **input)
{
	t_token	temp_token;
	char	*temp_move;
	char	*temp_str;

	temp_move = ft_strchr(*input, ' ');
	temp_token = create_token(TOKEN_WORD, *input);
	if (temp_move)
	{
		*temp_move = 0;
		*input = temp_move + 1;
	}
	else
		*input = ft_strchr(*input, 0);
	return (temp_token);
}
