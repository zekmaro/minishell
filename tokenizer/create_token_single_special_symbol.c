/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_token_single_special_symbol.c               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 12:36:57 by anarama           #+#    #+#             */
/*   Updated: 2024/07/21 12:37:35 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_single_special(const char *input)
{
	return ((*input == '<')
			|| (*input == '>')
			|| (*input == '|'));
}

t_token	*create_token_single_special_symbol(const char **input)
{
	t_token	*temp_token;
	
	temp_token = NULL;
	if (**input == '<') 
	{
		temp_token = create_token(TOKEN_REDIRECT_OUT, "<");
		(*input)++;
	} 
	else if (**input == '>') 
	{
		temp_token = create_token(TOKEN_REDIRECT_IN, ">");
		(*input)++;
	} 
	else if (**input == '|') 
	{
		temp_token = create_token(TOKEN_PIPE, "|");
		(*input)++;
	}
	return (temp_token);
}
