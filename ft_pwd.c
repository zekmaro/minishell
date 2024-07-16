/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 11:52:48 by andrejarama       #+#    #+#             */
/*   Updated: 2024/07/16 11:44:12 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_pwd(int fd, char **env)
{
	char	*pwd;

	pwd = getenv("PWD");
	write(fd, pwd, ft_strlen(pwd));
	write(fd, "\n", 1);
}
