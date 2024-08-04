/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 13:56:33 by vvobis            #+#    #+#             */
/*   Updated: 2024/08/04 10:31:19 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_env(const char **environment, int32_t *exit_status)
{
	if (!environment)
		*exit_status = 1;
	else
	{
		*exit_status = 0;
		environment_print(environment);
	}
}
