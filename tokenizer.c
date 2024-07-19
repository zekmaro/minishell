/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 19:49:35 by anarama           #+#    #+#             */
/*   Updated: 2024/07/19 15:33:58 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	ft_free_2(void *addr_ptr)
// {
// 	void **temp;

// 	temp = (void **)addr_ptr;
// 	free(*temp);
// 	*temp = NULL;
// }

// void	free_token(void *addr_token)
// {
// 	t_token *temp;

// 	temp = (t_token *)addr_token;
// 	ft_free_2(&temp->token_value);
// 	ft_free_2(&temp);
// }

void	free_tokens_arr(void *addr_tokens)
{
	t_token **temp;
	t_token **save_head_addr;

	temp = (t_token **)addr_tokens;
	save_head_addr = temp;
	while (*temp)
	{
		// free_token(*temp);
		free((*temp)->token_value);
		(*temp)->token_value = NULL;
		free(*temp);
		*temp = NULL;
		temp++;
	}
	free(addr_tokens);
}

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
	if (c == '\"' && second_double_found)
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

int get_len_next_space_or_quote(const char *input) 
{
    int i = 0;
    while (input[i] && !ft_isspace(input[i]) && !ft_is_double_quote(input[i], NULL))
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

void	copy_with_new_env_var(char *new_input, const char *original_input, char *env_string, int initial_var_len)
{
	int i;
	int j;

	i = 0;
	while (original_input[i] != '$')
	{
		new_input[i] = original_input[i];
		i++;
	}
	j = i + initial_var_len + 1;
	if (!env_string)
		return ;
	while (*env_string)
	{
		new_input[i] = *env_string;
		env_string++;
		i++;
	}
	while (original_input[j] && original_input[j] != '"')
	{
		new_input[i] = original_input[j];
		i++;
		j++;
	}
}

int get_len_next_double_quote(const char *input, char **env, char **new_input)
{
    int i = 0;
    int second_double_found = 0;
	int initial_var_len;
	char *env_string;

	env_string = NULL;
    while (input[i] && !ft_is_double_quote(input[i], &second_double_found))
	{
		if (input[i] == '$')
		{
			initial_var_len = get_len_next_space_or_quote(input + i + 1);
			env_string = ft_substr(input + i + 1, 0, initial_var_len);
			// if (!env_string)
			// {
			// 	perror("substr failed while expanding env var in quotes");
			// 	lst_memory(NULL, NULL, CLEAN);
			// }
			// else
			// 	lst_memory((void *)&env_string, ft_free_2, ADD);
            handle_dollar_sign(&env_string, env);
			*new_input = ft_calloc(ft_strlen(input) - initial_var_len + 1 + ft_strlen(env_string) + 1, sizeof(char));
			// if (!*new_input)
			// {
			// 	perror("substr failed while expanding env var in quotes");
			// 	lst_memory(NULL, NULL, CLEAN);
			// }
			// else
			// 	lst_memory((void *)*new_input, free, ADD);
			copy_with_new_env_var(*new_input, input, env_string, initial_var_len);
		}
		i++;
	}
    if (second_double_found == 0)
	{
		return (0);
	}
    return (i + 1);
}

void **custom_realloc(void **tokens, int old_capacity, int new_capacity)
{
	int		i;
    void	**new_tokens;

	new_tokens = ft_calloc((new_capacity + 1) , sizeof(void *));
	if (!new_tokens) 
	{
        perror("malloc failed while reallocing memory");
        lst_memory(NULL, NULL, CLEAN);
    }
	else
		lst_memory(new_tokens, free_tokens_arr, ADD);
	i = 0;
	while (i < old_capacity)
	{
		new_tokens[i] = tokens[i];
		i++;	
	}
    //free_tokens_arr(tokens);
    return (new_tokens);
}

t_token *create_token(t_token_type token_type, const char *value) 
{
    t_token	*token;

	token = malloc(sizeof(t_token));
	// if (!token) 
	// {
    //     perror("malloc failed while creating token");
    //     lst_memory(NULL, NULL, CLEAN);
    // }
	// else
	// 	lst_memory(token, free_token, ADD);
    token->token_type = token_type;
    token->token_value = ft_strdup(value);
	// if (!token->token_value) 
	// {
    //     perror("malloc in strdup failed while creating token");
    //     lst_memory(NULL, NULL, CLEAN);
    // }
	// else
	// 	lst_memory(token->token_value, free, ADD);
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

// do i need to free all the tokens and then again do it in the free tokens arr? or if i added all single tokens
// in lst_mem then i can just free the double arr without the inside tokens?

t_token	**lexical_analysis(const char *input, char **env)
{
    int capacity = INITIAL_TOKEN_CAPACITY;
    int count = 0;
    t_token **tokens;
	t_token *temp_token = NULL;
    int temp;
    char *temp_str;
	char *new_input;
	
	tokens = ft_calloc(1 , (capacity + 1) * sizeof(t_token *));
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
		//printf("%s\n", input);
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
            temp_str = ft_substr(input + 1, 0, temp);
			if (!temp_str)
			{
				perror("substr failed while expanding env var");
				lst_memory(NULL, NULL, CLEAN);
			}
			else
				lst_memory((void *)temp_str, free, ADD);
            handle_dollar_sign(&temp_str, env);
            temp_token = create_token(TOKEN_ENV, temp_str);
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
			else
				lst_memory((void *)temp_str, free, ADD);
            temp_token = create_token(TOKEN_WORD, temp_str);
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
				else
					lst_memory((void *)temp_str, free, ADD);
				temp_token = create_token(TOKEN_WORD, temp_str);
			}
			else
				temp_token = create_token(TOKEN_WORD, new_input);
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
			else
				lst_memory((void *)temp_str, free, ADD);
            temp_token = create_token(TOKEN_WORD, temp_str);
            input += temp;
        }
        if (!temp_token) 
		{
            perror("Token creation failed");
            lst_memory(NULL, NULL, CLEAN);
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