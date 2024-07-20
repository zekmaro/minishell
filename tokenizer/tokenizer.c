/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 19:49:35 by anarama           #+#    #+#             */
/*   Updated: 2024/07/20 19:47:14 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	**lexical_analysis(const char *input, char **env)
{
    int capacity = INITIAL_TOKEN_CAPACITY;
    int count = 0;
    t_token **tokens;
	t_token *temp_token = NULL;
    int temp;
    char *temp_str;
	char *new_input;
	
	tokens = ft_calloc((capacity + 1), sizeof(t_token *));
	if (!tokens) 
	{
        perror("malloc in calloc for initialising tokens");
        lst_memory(NULL, NULL, CLEAN);
    }
	else
		lst_memory((void *)tokens, free_tokens_arr, ADD);
    while (*input) 
	{
		temp = 0;
        temp_str = NULL;
		new_input = NULL;
        while (ft_isspace(*input))
            input++;
        if (*input == '\0')
            break ;
		if (ft_strncmp(input, ">>", 2) == 0) 
		{
            temp_token = create_token(TOKEN_REDIRECT_APPEND, ">>");
            input += 2;
        } 
		else if (ft_strncmp(input, "<<", 2) == 0) 
		{
            temp_token = create_token(TOKEN_HEREDOC, "<<");
            input += 2;
        } 
		else if (ft_strncmp(input, "&&", 2) == 0) 
		{
            temp_token = create_token(TOKEN_AND, "&&");
            input += 2;
        } 
		else if (ft_strncmp(input, "||", 2) == 0) 
		{
            temp_token = create_token(TOKEN_OR, "||");
            input += 2;
        } 
		else if (*input == '<') 
		{
            temp_token = create_token(TOKEN_REDIRECT_OUT, "<");
            input++;
        } 
		else if (*input == '>') 
		{
            temp_token = create_token(TOKEN_REDIRECT_IN, ">");
            input++;
        } 
		else if (*input == '|') 
		{
            temp_token = create_token(TOKEN_PIPE, "|");
            input++;
        } 
		else if (*input == '$') 
		{
            temp = get_len_next_space(input + 1);
            temp_str = ft_substr(input + 1, 0, temp);
			if (!temp_str)
			{
				perror("substr failed while expanding env var");
				lst_memory(NULL, NULL, CLEAN);
			}
            handle_dollar_sign(&temp_str, env);
            temp_token = create_token(TOKEN_ENV, temp_str);
			free(temp_str);
            input += temp + 1;
        } 
		else if (*input == '\'') 
		{
            temp = get_len_next_single_quote(input + 1);
			temp_str = ft_substr(input + 1, 0, temp - 1);
			if (!temp_str)
			{
				perror("substr failed in single quote");
				lst_memory(NULL, NULL, CLEAN);
			}
            temp_token = create_token(TOKEN_WORD, temp_str);
			free(temp_str);
            input += temp + 1;
        } 
		else if (*input == '"') 
		{
            temp = get_len_next_double_quote(input + 1, env, &new_input);
			if (!new_input)
			{
				temp_str = ft_substr(input + 1, 0, temp - 1);
				if (!temp_str)
				{
					perror("substr failed in single quote");
					lst_memory(NULL, NULL, CLEAN);
				}
				temp_token = create_token(TOKEN_WORD, temp_str);
				free(temp_str);
			}
			else
			{
				temp_token = create_token(TOKEN_WORD, new_input);
				free(new_input);	
			}
            input += temp + 1;
        } 
		else 
		{
            temp = get_len_next_special_char(input);
			temp_str = ft_substr(input, 0, temp);
			if (!temp_str)
			{
				perror("substr failed in single quote");
				lst_memory(NULL, NULL, CLEAN);
			}
            temp_token = create_token(TOKEN_WORD, temp_str);
			free(temp_str);
            input += temp;
        }
        if (!temp_token) 
		{
            perror("Token creation failed");
            lst_memory(NULL, NULL, CLEAN);
		}
        if (count >= capacity)
		{
			tokens = (t_token **)custom_realloc((void **)tokens, capacity, capacity * 2, 1);
			capacity *= 2;
		}
        tokens[count++] = temp_token;
		temp_token = NULL;
    }
    return (tokens);
}

// capacity out of int range?