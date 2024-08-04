/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 16:52:07 by vvobis            #+#    #+#             */
/*   Updated: 2024/08/04 10:14:57 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	check_exit_status(const char *status)
{
	uint32_t	i;

	i = 0;
	while (ft_isspace(status[i]))
		i++;
}

void	ft_exit(const char **args)
{
	uint32_t	args_length;
	int32_t		exit_status;

	exit_status = 0;
	args_length = get_split_size(args);
	if (args_length == 2)
	{
		lst_memory(NULL, NULL, END);
		exit_status = ft_atoi(args[1]);
	}
}
