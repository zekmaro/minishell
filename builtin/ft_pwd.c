/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor </var/spool/mail/victor>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 22:49:03 by victor            #+#    #+#             */
/*   Updated: 2024/08/04 10:32:02 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_pwd(const char **environment, int32_t *exit_status)
{
	char	*pwd;

	if (!environment)
		*exit_status = 1;
	else
	{
		*exit_status = 0;
		pwd = environment_variable_value_get("PWD", environment);
		ft_putendl_fd(pwd, 1);
	}
}
