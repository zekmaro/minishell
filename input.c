/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 16:18:42 by vvobis            #+#    #+#             */
/*   Updated: 2024/07/08 12:02:02 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_split(void *back)
{
	char	**free_back;

	if (!back)
		return ;
	free_back = (char **)back;
	while (*free_back)
	{
		ft_free((void **)free_back);
		free_back++;
	}
	ft_free(&back);
	return ;
}
