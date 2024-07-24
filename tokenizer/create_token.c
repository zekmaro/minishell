/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 19:47:21 by anarama           #+#    #+#             */
/*   Updated: 2024/07/22 13:07:13 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	create_token(t_token_type token_type, const char *value)
{
	t_token	token;

	token.token_type = token_type;
	token.token_value = (char *)value;
	return (token);
}
