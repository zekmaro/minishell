/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_token_quotes.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 12:38:47 by anarama           #+#    #+#             */
/*   Updated: 2024/07/21 12:39:29 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_quote(const char *input)
{
	return (*input == '\'' || *input == '"');
}

t_token	*create_token_single_quote(const char **input, const char **env)
{
	t_token	*temp_token;
	int 	temp;
	char	*temp_str;

	temp_token = NULL;
	temp = get_len_next_single_quote(*input + 1);
	temp_str = ft_substr(*input + 1, 0, temp - 1);
	if (!temp_str)
	{
		perror("substr failed in single quote");
		lst_memory(NULL, NULL, CLEAN);
	}
	lst_memory(temp_str, free, ADD);
	temp_token = create_token(TOKEN_WORD, temp_str);
	*input += temp + 1;
	return (temp_token);
}
t_token	*create_token_double_quotes(const char **input, const char **env)
{
	t_token	*temp_token;
	int 	temp;
	char	*temp_str;
	char 	*new_input;

	temp_token = NULL;
	new_input = NULL;
	temp = get_len_next_double_quote(*input + 1, (char **)env, &new_input);
	if (!new_input)
	{
		temp_str = ft_substr(*input + 1, 0, temp - 1);
		if (!temp_str)
		{
			perror("substr failed in single quote");
			lst_memory(NULL, NULL, CLEAN);
		}
		lst_memory(temp_str, free, ADD);
		temp_token = create_token(TOKEN_WORD, temp_str);
	}
	else
	{
		lst_memory(new_input, free, ADD);
		temp_token = create_token(TOKEN_WORD, new_input);
	}
	*input += temp + 1;
	return (temp_token);
}

t_token	*create_token_quotes(const char **input, const char **env)
{
	t_token	*temp_token;

	temp_token = NULL;
	if (**input == '\'') 
	{
		temp_token = create_token_single_quote(input, env);
	}
	else if (**input == '"') 
	{
		temp_token = create_token_double_quotes(input, env);
	} 
	return (temp_token);
}
