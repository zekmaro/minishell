/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 11:39:04 by vvobis            #+#    #+#             */
/*   Updated: 2024/07/25 16:13:37 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int32_t	ft_echo(char **args)
{
	uint32_t	i;

	i = 1;
	if (!args[i])
		return (ft_putchar_fd('\n', 1), 0);
	if (ft_strncmp(args[i], "-n", 3) == 0)
	{
		i++;
		while (args[i])
		{
			ft_putstr_fd(args[i], 1);
			i++;
		}
	}
	else
	{
		while (args[i + 1])
		{
			ft_putstr_fd(args[i], 1);
			ft_putchar_fd(' ', 1);
			i++;
		}
		ft_putendl_fd(args[i], 1);
	}
	return (0);
}
