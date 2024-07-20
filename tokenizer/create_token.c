/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 19:47:21 by anarama           #+#    #+#             */
/*   Updated: 2024/07/20 19:49:27 by anarama          ###   ########.fr       */
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
        perror("malloc in strdup failed while creating token");
        lst_memory(NULL, NULL, CLEAN);
    }
    return (token);
}

void	copy_token_info(void **dest, t_token *src)
{
	char	*temp;

	*dest = create_token(src->token_type, src->token_value);
	if (!*dest)
	{
		perror("Token creation failed");
        lst_memory(NULL, NULL, CLEAN);
	}
}
