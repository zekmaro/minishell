/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 19:49:35 by anarama           #+#    #+#             */
/*   Updated: 2024/07/30 14:41:51 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_tokens(void *token_ptr)
{
	t_token		*token;
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
	lst_memory(tokens, free_tokens, ADD);
	return (tokens);
}

static t_token	check_symbol_and_create_token(const char **input,
					const char **env)
{
	if (is_double_special(*input))
		return (create_token_double_special_symbol((char **)input));
	else if (is_single_special(**input))
		return (create_token_single_special_symbol(input));
	else if (is_env_var(**input))
		return (create_token_env_var((char **)input, env));
	else if (is_quote(**input))
		return (create_token_quotes(input, env));
	else
		return (create_token_word(input));
}

static bool	is_token(char c)
{
	return (c == '>' || c == '|' || c == '<' || ft_isalnum(c));
}

static uint32_t	get_word_count(char *input)
{
	uint32_t	i;
	uint32_t	word_count;
	char		store_current_char;

	if (!input)
		return (0);
	i = 0;
	word_count = 0;
	while (input[i])
	{
		while (input[i] && ft_isspace(input[i]))
			i++;
		if (is_double_special(&input[i]))
			i += 2;
		else if (is_special_char(input[i]))
			i++;
		else if (is_quote(input[i]))
		{
			store_current_char = input[i++];
			while (input[i] && input[i] != store_current_char)
				i++;
		}
		else
		{
			while (input[i] && !ft_isspace(input[i]) \
					&& !is_double_special(&input[i]) \
					&& !is_single_special(input[i]) \
					&& !is_quote(input[i]))
				i++;
		}
		word_count++;
	}
	return (word_count);
}

void	print_tokens(t_token *token)
{
	uint32_t	i;

	i = 0;
	while (token[i].token_type != TOKEN_EOL)
		ft_putstr_fd(token[i++].token_value, 1);
}

t_token	*lexical_analysis(const char *input, const char **env)
{
	t_token		*tokens;
	uint32_t	i;
	char		*input_free;

	tokens = initialise_tokens(get_word_count((char *)input));
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
