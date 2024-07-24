/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 19:49:35 by anarama           #+#    #+#             */
/*   Updated: 2024/07/24 11:52:04 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_tokens(void *token_ptr)
{
	t_token	*token;
	uint32_t	i;

	token = token_ptr;
	i = 0;
	while (token[i].token_type != TOKEN_EOL)
	{
		if (token[i].token_type == TOKEN_ENV)
			ft_free((void **)&token[i].token_value);
		i++;
	}
	ft_free((void **)&token);
}

static t_token	*initialise_tokens(uint32_t word_count)
{
	t_token	*tokens;

	tokens = ft_calloc(word_count + 1, sizeof(t_token));
	tokens[word_count].token_type = TOKEN_EOL;
	lst_memory(tokens, free_tokens, ADD);
	return (tokens);
}

static t_token	check_symbol_and_create_token(const char **input,
					const char **env)
{
	if (is_double_special(*input))
		return (create_token_double_special_symbol((char **)input));
	else if (is_single_special(*input))
		return (create_token_single_special_symbol(input));
	else if (is_env_var(*input))
		return (create_token_env_var((char **)input, env));
	else if (is_quote(*input))
		return (create_token_quotes(input, env));
	else
		return (create_token_word(input));
}

static uint32_t	get_word_count(const char *input)
{
	uint32_t	i;
	uint32_t	word_count;

	if (!input)
		return (0);
	i = 0;
	word_count = 1;
	while (input[i])
	{
		while (input[i] && ft_isspace(input[i]))
			i++;
		word_count++;
		while (input[i] && !ft_isspace(input[i]))
			i++;
	}
	return (word_count);
}

t_token	*lexical_analysis(const char *input, const char **env)
{
	t_token		*tokens;
	uint32_t	i;

	tokens = initialise_tokens(get_word_count(input));
	i = 0;
	while (*input)
	{
		while (ft_isspace(*input))
			input++;
		if (*input == '\0')
			break ;
		tokens[i++] = check_symbol_and_create_token(&input, env);
	}
	tokens[i].token_type = TOKEN_EOL;
	return (tokens);
}
