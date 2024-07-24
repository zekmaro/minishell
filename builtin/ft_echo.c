/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 11:39:04 by vvobis            #+#    #+#             */
/*   Updated: 2024/07/24 13:49:58 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int32_t	ft_echo(t_ast *node)
{
	uint32_t	i;

	i = 1;
	if (ft_strncmp(node->args[i], "-n", 3) == 0)
	{
		i++;
		while (node->args[i])
		{
			ft_putstr_fd(node->args[i], 1);
			i++;
		}
	}
	else
	{
		while (node->args[i + 1])
		{
			ft_putstr_fd(node->args[i], 1);
			ft_putchar_fd(' ', 1);
			i++;
		}
		ft_putendl_fd(node->args[i], 1);
	}
	return (0);
}
