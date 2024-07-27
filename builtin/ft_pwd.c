/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor </var/spool/mail/victor>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 22:49:03 by victor            #+#    #+#             */
/*   Updated: 2024/07/27 11:18:30 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_pwd(const char **environment)
{
	char	*pwd;

	pwd = environment_variable_value_get("PWD", environment);
	ft_putendl_fd(pwd, 1);
}
