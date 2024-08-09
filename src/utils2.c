/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 13:50:41 by anarama           #+#    #+#             */
/*   Updated: 2024/08/02 17:31:51 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	custom_memmove_strings(char **dest, char **src)
{
	char	*temp;

	temp = NULL;
	while (*src)
	{
		temp = ft_strdup(*src);
		if (!temp)
		{
			lst_memory(NULL, NULL, CLEAN);
		}
		free(*dest);
		*dest = temp;
		temp = NULL;
		dest++;
		src++;
	}
	while (*dest)
	{
		free(*dest);
		*dest = NULL;
		dest++;
	}
}

int	get_tokens_count(char **tokens)
{
	int	i;

	i = 0;
	while (*tokens)
	{
		tokens++;
		i++;
	}
	return (i);
}
