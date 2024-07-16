/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 19:18:13 by anarama           #+#    #+#             */
/*   Updated: 2024/07/09 22:55:07 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_chdir(const char *path)
{
	if (path == NULL)
	{
		path = getenv("HOME");
	}
	if (chdir(path) == -1)
	{
		perror("cd");
	}
}

void	ft_exit(void)
{
	lst_memory(NULL, NULL, CLEAN);
	terminal_raw_mode_disable();
	exit(0);
}
