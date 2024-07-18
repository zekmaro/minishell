/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 19:49:35 by anarama           #+#    #+#             */
/*   Updated: 2024/07/18 11:58:09 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_special_char(char c)
{
	return (c == ' ' || c == '|' || c == '>' || c == '<' || c == '$' || c == '\'' || c == '\"');
}

int	ft_isspace(char c)
{
	return (c == ' ');
}

int	ft_is_single_quote(char c, int *second_quote_found)
{
	if (c == '\'')
	{
		*second_quote_found = 1;
	}
	return (c == '\'');
}

int	ft_is_double_quote(char c, int *second_double_found)
{
	if (c == '\"')
	{
		*second_double_found = 1;
	}
	return (c == '\"');
}

int get_len_next_special_char(const char *input) 
{
    int i = 0;
    while (input[i] && !is_special_char(input[i]))
        i++;
    return (i);
}

int get_len_next_space(const char *input) 
{
    int i = 0;
    while (input[i] && !ft_isspace(input[i]))
        i++;
    return (i);
}

int get_len_next_single_quote(const char *input) 
{
    int i = 0;
    int second_quote_found = 0;

    while (input[i] && !ft_is_single_quote(input[i], &second_quote_found))
        i++;
    if (second_quote_found == 0)
        return (0);
    return (i + 1); // Include the closing quote
}

int get_len_next_double_quote(const char *input) 
{
    int i = 0;
    int second_double_found = 0;

    while (input[i] && !ft_is_double_quote(input[i], &second_double_found))
        i++;
    if (second_double_found == 0)
        return (0);
    return (i + 1);
}

void **custom_realloc(void **tokens, int old_capacity, int new_capacity)
{
	int i;
    void **new_tokens = ft_calloc((new_capacity + 1) , sizeof(void *));
    if (!new_tokens) 
	{
        perror("malloc");
        exit(1);
    }
	i = 0;
	while (i < old_capacity)
	{
		new_tokens[i] = tokens[i];
		i++;	
	}
    free(tokens);
    return (new_tokens);
}

t_token *create_token(t_token_type token_type, const char *value) 
{
    t_token *token = malloc(sizeof(t_token));
    if (!token) 
	{
        perror("malloc");
        exit(1);
    }
    token->token_type = token_type;
    token->token_value = strdup(value);
    if (!token->token_value) 
	{
        perror("strdup");
        exit(1);
    }
    return (token);
}

void print_tokens(t_token **tokens) 
{
	int i;

	i = 0;
    while (tokens[i])
	{
        printf("Token: Type=%d, Value=%s\n", tokens[i]->token_type, tokens[i]->token_value);
		i++;
	}
}

t_token	**lexical_analysis(const char *input, char **env)
{
    int capacity = INITIAL_TOKEN_CAPACITY;
    int count = 0;
    t_token **tokens;
	t_token *temp_token = NULL;
    int temp = 0;
    char *string = NULL;
	
	tokens = ft_calloc(1 , (capacity + 1) * sizeof(t_token *));
	if (!tokens) 
	{
        perror("malloc");
        exit(1);
    }

    while (*input) 
	{
        while (ft_isspace(*input))
            input++;
        if (*input == '\0')
            break ;
        temp = 0;
        string = NULL;
		if (strncmp(input, ">>", 2) == 0) 
		{
            temp_token = create_token(TOKEN_REDIRECT_APPEND, ">>");
            input += 2;
        } 
		else if (strncmp(input, "<<", 2) == 0) 
		{
            temp_token = create_token(TOKEN_HEREDOC, "<<");
            input += 2;
        } 
		else if (strncmp(input, "&&", 2) == 0) 
		{
            temp_token = create_token(TOKEN_AND, "&&");
            input += 2;
        } 
		else if (strncmp(input, "||", 2) == 0) 
		{
            temp_token = create_token(TOKEN_OR, "||");
            input += 2;
        } 
		else if (*input == '<') 
		{
            temp_token = create_token(TOKEN_REDIRECT_IN, "<");
            input++;
        } 
		else if (*input == '>') 
		{
            temp_token = create_token(TOKEN_REDIRECT_OUT, ">");
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
            string = strndup(input + 1, temp);
            handle_dollar_sign(&string, env);
            temp_token = create_token(TOKEN_ENV, string);
            input += temp + 1;
        } 
		else if (*input == '\'') 
		{
            temp = get_len_next_single_quote(input + 1);
            temp_token = create_token(TOKEN_WORD, strndup(input + 1, temp - 1));
            input += temp + 1;
        } 
		else if (*input == '"') 
		{
            temp = get_len_next_double_quote(input + 1);
            temp_token = create_token(TOKEN_WORD, strndup(input + 1, temp - 1));
            input += temp + 1;
        } 
		else 
		{
            temp = get_len_next_special_char(input);
            temp_token = create_token(TOKEN_WORD, strndup(input, temp));
            input += temp;
        }
        if (!temp_token) 
		{
            perror("Token creation failed");
            exit(1);
		}
        if (count >= capacity)
		{
			tokens = (t_token **)custom_realloc((void **)tokens, capacity, capacity * 2);
			capacity *= 2;
		}
        tokens[count++] = temp_token;
		temp_token = NULL;
    }
    return (tokens);
}

// capacity out of int range?