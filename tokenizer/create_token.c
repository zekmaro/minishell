/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 19:47:21 by anarama           #+#    #+#             */
/*   Updated: 2024/07/21 12:32:50 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token *create_token(t_token_type token_type, const char *value) 
{
    t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token) 
	{
        perror("malloc failed while creating token");
        lst_memory(NULL, NULL, CLEAN);
    }
    token->token_type = token_type;
    token->token_value = ft_strdup(value);
	if (!token->token_value) 
	{
		free(token);
        perror("malloc in strdup failed while creating token");
        lst_memory(NULL, NULL, CLEAN);
    }
    return (token);
}
