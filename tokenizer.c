/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 19:49:35 by anarama           #+#    #+#             */
/*   Updated: 2024/07/16 20:20:06 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isspace(char c)
{
	return (c == ' ');
}

//should i in the end put the whole list in the lst_mem?

t_tokens	*create_token(t_token_type token_type, const char *value)
{
	t_tokens	*token;

	token = malloc(sizeof(t_tokens));
	if (!token)
		lst_memory(NULL, NULL, CLEAN);
	token->token_type = token_type;
	token->token_value = value;
	token->next = NULL;
}

void	append_node(t_tokens **head, t_tokens *new_node)
{
	t_tokens	*temp;

	if (!*head)
	{
		*head = new_node;
	}
	else 
	{
		temp = *head;
		while (temp->next)
		{
			temp = temp->next;
		}
		temp->next = new_node;
	}
}

t_tokens	*lexical_analysis(const char *input)
{
	t_tokens	*head;
	t_tokens	*temp_token;

	head = NULL;
	temp_token = NULL;
	while (*input)
	{
		while (ft_isspace(*input))
			input++;
		if (ft_strncmp(input, ">>", 2))
		{
			temp_token = create_token(TOKEN_REDIRECT_APPEND, ">>");
			if (!temp_token)
				lst_memory(NULL, NULL, CLEAN);
		}
		else if (ft_strncmp(input, "<<", 2))
		{
			temp_token = create_token(TOKEN_HEREDOC, "<<");
			if (!temp_token)
				lst_memory(NULL, NULL, CLEAN);
		}
		else if (ft_strncmp(input, "&&", 2))
		{
			temp_token = create_token(TOKEN_AND, "&&");
			if (!temp_token)
				lst_memory(NULL, NULL, CLEAN);
		}
		else if (ft_strncmp(input, "||", 2))
		{
			temp_token = create_token(TOKEN_OR, "||");
			if (!temp_token)
				lst_memory(NULL, NULL, CLEAN);
		}
		else if (ft_strchr("<>|", *input))
		{
			if (*input == '<')
				temp_token = create_token(TOKEN_REDIRECT_OUT, "|");
			else if (*input == '>')
				temp_token = create_token(TOKEN_REDIRECT_IN, ">");
			else if (*input == '|')
				temp_token = create_token(TOKEN_PIPE, "|");
			if (!temp_token)
				lst_memory(NULL, NULL, CLEAN);
		}
		append_node(&head, temp_token);
		temp_token = NULL;
	}
}
