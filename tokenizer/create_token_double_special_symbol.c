/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_token_double_special_symbol.c               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 12:35:12 by anarama           #+#    #+#             */
/*   Updated: 2024/07/21 16:49:26 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_double_special(const char *input)
{
	return ((ft_strncmp(input, ">>", 2) == 0)
		|| (ft_strncmp(input, "<<", 2) == 0)
		|| (ft_strncmp(input, "&&", 2) == 0)
		|| (ft_strncmp(input, "||", 2) == 0));
}

t_token	*create_token_double_special_symbol(const char **input)
{
	t_token	*temp_token;

	temp_token = NULL;
	if (ft_strncmp(*input, ">>", 2) == 0)
	{
		temp_token = create_token(TOKEN_REDIRECT_APPEND, ">>");
		*input += 2;
	}
	else if (ft_strncmp(*input, "<<", 2) == 0)
	{
		temp_token = create_token(TOKEN_HEREDOC, "<<");
		*input += 2;
	}
	else if (ft_strncmp(*input, "&&", 2) == 0)
	{
		temp_token = create_token(TOKEN_AND, "&&");
		*input += 2;
	}
	else if (ft_strncmp(*input, "||", 2) == 0)
	{
		temp_token = create_token(TOKEN_OR, "||");
		*input += 2;
	}
	return (temp_token);
}
