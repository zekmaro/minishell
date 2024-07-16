/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 11:52:48 by andrejarama       #+#    #+#             */
/*   Updated: 2024/07/15 12:32:23 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

static void	custom_memmove_chars(char *dest, char *src)
{
	while (*src)
	{
		*dest = *src;
		dest++;
		src++;
	}
	while (*dest)
	{
		*dest = '\0';
		dest++;
	}
}

void	ft_pwd(int fd, char **env)
{
	char	*pwd;

	pwd = NULL;
	while (*env)
	{
		if (ft_strncmp("PWD", *env, 3) == 0)
		{
			pwd = *env;
			break;
		}
		env++;
	}
	custom_memmove_chars(pwd, pwd + 4);
	write(fd, pwd, ft_strlen(pwd));
	write(fd, "\n", 1);
}
