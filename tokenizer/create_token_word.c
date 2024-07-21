/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_token_word.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 12:40:09 by anarama           #+#    #+#             */
/*   Updated: 2024/07/21 12:40:30 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*create_token_word(const char **input)
{
	t_token	*temp_token;
	int 	temp;
	char	*temp_str;

	temp_token = NULL;
	temp = get_len_next_special_char(*input);
	temp_str = ft_substr(*input, 0, temp);
	if (!temp_str)
	{
		perror("substr failed in single quote");
		lst_memory(NULL, NULL, CLEAN);
	}
	lst_memory(temp_str, free, ADD);
	temp_token = create_token(TOKEN_WORD, temp_str);
	*input += temp;
	return (temp_token);
}
