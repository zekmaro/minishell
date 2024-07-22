/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_token_quotes.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 12:38:47 by anarama           #+#    #+#             */
/*   Updated: 2024/07/22 20:58:00 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_quote(const char *input)
{
	return (*input == '\'' || *input == '\"');
}

t_token	create_token_single_quote(const char **input)
{
	t_token	temp_token;
	char	*temp_move;

	temp_move = ft_strchr(*input + 1, '\'');
	temp_token = create_token(TOKEN_WORD, (*input) + 1);
	if (temp_move)
	{
		*temp_move = 0;
		*input = temp_move + 1;
	}
	else
		*input = ft_strchr(*input, 0);
	return (temp_token);
}

t_token	create_token_double_quotes(const char **input, const char **env)
{
	t_token	temp_token;
	char	*temp_str;
	char	*new_input;

	new_input = NULL;
	get_len_next_double_quote(*input + 1, env, &new_input);
	if (!new_input)
	{
		temp_str = ft_strchr(*input + 1, '\"');
		if (temp_str)
		{
			*temp_str = 0;
			temp_str = ft_strrchr(*input + 1, '\"');
			*input += ft_strlen(*input) + 1;
		}
		temp_token = create_token(TOKEN_WORD, temp_str);
	}
	else
	{
		temp_token = create_token(TOKEN_ENV, new_input);
	}
	return (temp_token);
}

t_token	create_token_quotes(const char **input, const char **env)
{
	t_token	temp_token;

	temp_token = (t_token){0};
	if (**input == '\'')
	{
		temp_token = create_token_single_quote(input);
	}
	else if (**input == '\"')
	{
		temp_token = create_token_double_quotes(input, env);
	}
	return (temp_token);
}
