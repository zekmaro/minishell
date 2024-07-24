/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_logical_operator.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 13:31:08 by anarama           #+#    #+#             */
/*   Updated: 2024/07/24 13:34:52 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_logical_operator(t_ast **logical_node, int exit_status)
{
	if ((*logical_node)->token_type == TOKEN_AND)
	{
		if (exit_status == 1)
		{
			if ((*logical_node)->right)
				(*logical_node) = (*logical_node)->right;
		}
	}
	else if ((*logical_node)->token_type == TOKEN_OR)
	{
		if (exit_status == 0)
		{
			if ((*logical_node)->right)
				(*logical_node) = (*logical_node)->right;
		}
	}
}
