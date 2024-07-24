/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 15:48:25 by andrejarama       #+#    #+#             */
/*   Updated: 2024/07/23 16:33:35 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_not_special_token(char *token)
{
	if (ft_strncmp(token, ">", ft_strlen(token)) == 0)
		return (0);
	else if (ft_strncmp(token, "<", ft_strlen(token)) == 0)
		return (0);
	else if (ft_strncmp(token, ">>", ft_strlen(token)) == 0)
		return (0);
	else if (ft_strncmp(token, "<<", ft_strlen(token)) == 0)
		return (0);
	else if (ft_strncmp(token, "|", ft_strlen(token)) == 0)
		return (0);
	else if (ft_strncmp(token, "||", ft_strlen(token)) == 0)
		return (0);
	else if (ft_strncmp(token, "&&", ft_strlen(token)) == 0)
		return (0);
	return (1);
}

void	ft_echo(char **tokens, int fd)
{
	tokens++;
	while (*tokens && is_not_special_token(*tokens))
	{
		write(fd, *tokens, ft_strlen(*tokens));
		if (*(tokens + 1) != NULL)
		{
			write(fd, " ", 1);
		}
		tokens++;
	}
	write(fd, "\n", 1);
}
