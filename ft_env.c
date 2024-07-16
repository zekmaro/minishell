/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 13:46:30 by anarama           #+#    #+#             */
/*   Updated: 2024/07/15 16:09:20 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

void	ft_env(char **env, int fd)
{
	while (*env)
	{
		write(fd, *env, ft_strlen(*env));
		write(fd, "\n", 1);
		env++;
	}
}
