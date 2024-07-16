/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrejarama <andrejarama@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 15:48:25 by andrejarama       #+#    #+#             */
/*   Updated: 2024/07/11 12:11:17 by andrejarama      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"

int		is_not_special_token(char *token)
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
		if (*(tokens +  1) != NULL)
		{
			write(fd, " ", 1);
		}
		tokens++;
	}
	// if (*(tokens +  1) != NULL
	// 	&& ft_strncmp(*(tokens +  1), "-n", ft_strlen(*(tokens +  1))) != 0)
	write(fd, "\n", 1);
}

// handle -n where i need to execute it but not print -n itself