/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 11:52:48 by andrejarama       #+#    #+#             */
/*   Updated: 2024/07/24 09:20:21 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_pwd(int fd, const char **env)
{
	char	*pwd;

	pwd = environment_variable_get("PWD", env);
	write(fd, pwd, ft_strlen(pwd));
	write(fd, "\n", 1);
}
