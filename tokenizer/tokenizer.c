/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 19:49:35 by anarama           #+#    #+#             */
/*   Updated: 2024/07/21 16:31:50 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	**initialise_tokens(void)
{
	int		capacity;
	t_token	**tokens;

	capacity = INITIAL_TOKEN_CAPACITY;
	tokens = ft_calloc((capacity + 1), sizeof(t_token *));
	if (!tokens)
	{
		perror("malloc in calloc for initialising tokens");
		lst_memory(NULL, NULL, CLEAN);
	}
	else
		lst_memory((void *)tokens, free_tokens_arr, ADD);
	return (tokens);
}

void	check_symbol_and_create_token(const char **input,
					const char **env, t_token	**temp_token)
{
	if (is_double_special(*input))
		*temp_token = create_token_double_special_symbol(input);
	else if (is_single_special(*input))
		*temp_token = create_token_single_special_symbol(input);
	else if (is_env_var(*input))
		*temp_token = create_token_env_var(input, env);
	else if (is_quote(*input))
		*temp_token = create_token_quotes(input, env);
	else
		*temp_token = create_token_word(input);
}

void	add_token_to_arr(t_token ***tokens, int *count,
			t_token *temp_token, int *capacity)
{
	(*tokens)[*count] = temp_token;
	if (*count + 1 >= *capacity)
	{
		*tokens = (t_token **)custom_realloc((void **)*tokens,
				*capacity, *capacity * 2, 1);
		*capacity *= 2;
	}
	(*count)++;
}

t_token	**lexical_analysis(const char *input, const char **env)
{
	int		capacity;
	int		count;
	t_token	**tokens;
	t_token	*temp_token;

	tokens = initialise_tokens();
	capacity = INITIAL_TOKEN_CAPACITY;
	count = 0;
	while (*input)
	{
		temp_token = NULL;
		while (ft_isspace(*input))
			input++;
		if (*input == '\0')
			break ;
		check_symbol_and_create_token(&input, env, &temp_token);
		if (!temp_token)
		{
			perror("Token creation failed");
			lst_memory(NULL, NULL, CLEAN);
		}
		add_token_to_arr(&tokens, &count, temp_token, &capacity);
	}
	return (tokens);
}
