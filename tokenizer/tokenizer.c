/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 19:49:35 by anarama           #+#    #+#             */
/*   Updated: 2024/07/21 13:31:45 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	**initialise_tokens()
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
		if (is_double_special(input))
			temp_token = create_token_double_special_symbol(&input);
		else if (is_single_special(input))
			temp_token = create_token_single_special_symbol(&input);
		else if (is_env_var(input))
			temp_token = create_token_env_var(&input, env);
		else if (is_quote(input))
			temp_token = create_token_quotes(&input, env);
		else 
		{
            temp_token = create_token_word(&input);
        }
        if (!temp_token) 
		{
            perror("Token creation failed");
            lst_memory(NULL, NULL, CLEAN);
		}
		// i first add the token and then do realloc if needed cause realloc might fail
		// before i added token to the arr. if i try to add it to lst_mem independently i will
		// get double free
		tokens[count] = temp_token;
        if (count + 1 >= capacity)
		{
			tokens = (t_token **)custom_realloc((void **)tokens, capacity, capacity * 2, 1);
			capacity *= 2;
		}
		count++;
    }
    return (tokens);
}

// capacity out of int range?