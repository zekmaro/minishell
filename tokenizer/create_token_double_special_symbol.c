/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_token_double_special_symbol.c               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 12:35:12 by anarama           #+#    #+#             */
/*   Updated: 2024/07/22 12:24:11 by vvobis           ###   ########.fr       */
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

t_token	create_token_double_special_symbol(char **input)
{
	t_token		temp_token;
	char		*manipulation_pointer;
	t_token_type token_type;
	uint32_t	i;

	temp_token = (t_token){0};
	i = 0;
	while (*input[i] && *input[i] == ' ')
		i++;
	manipulation_pointer = ft_strchr(&(*input)[i], ' ');
	if (manipulation_pointer)
		*manipulation_pointer = 0;
	if (ft_strncmp(*input, ">>", 2) == 0)
	{
		token_type = TOKEN_REDIRECT_APPEND;
	}
	else if (ft_strncmp(*input, "<<", 2) == 0)
	{
		token_type = TOKEN_HEREDOC;
	}
	else if (ft_strncmp(*input, "&&", 2) == 0)
	{
		token_type = TOKEN_AND;
	}
	else if (ft_strncmp(*input, "||", 2) == 0)
	{
		token_type = TOKEN_OR;
	}
	temp_token = create_token(token_type, manipulation_pointer);
	*input = manipulation_pointer + 1;
	return (temp_token);
}
