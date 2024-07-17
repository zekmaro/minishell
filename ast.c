/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 13:17:47 by anarama           #+#    #+#             */
/*   Updated: 2024/07/17 20:31:03 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//cleaning mmeory like this wont work in the proccess i might fail
// if one of allocation failed clean the whole list is cleaned but if all success i put head in lstmem
// add each node in the lst mem?

t_ast	*create_command_node(char **args)
{
	t_ast	*node;

	node = ft_calloc(1, (sizeof(t_ast)));
	if (!node)
		lst_memory(NULL, NULL, CLEAN);
	node->type = NODE_COMMAND;
	node->args = args;
	node->fd_in = STDIN_FILENO;
	node->fd_out = STDOUT_FILENO;
	return (node);
}

t_ast	*create_redireciton_node(char *file_name)
{
	t_ast	*node;

	node = ft_calloc(1, (sizeof(t_ast)));
	if (!node)
		lst_memory(NULL, NULL, CLEAN);
	node->type = NODE_REDIRECTION;
	node->file = file_name;
	node->fd_in = STDIN_FILENO;
	node->fd_out = STDOUT_FILENO;
	return (node);
}

t_ast	*create_pipe_node()
{
	t_ast	*node;

	node = ft_calloc(1, (sizeof(t_ast)));
	if (!node)
		lst_memory(NULL, NULL, CLEAN);
	node->type = NODE_PIPE;
	node->fd_in = STDIN_FILENO;
	node->fd_out = STDOUT_FILENO;
	return (node);
}

t_ast	*create_logical_node()
{
	t_ast	*node;

	node = ft_calloc(1, (sizeof(t_ast)));
	if (!node)
		lst_memory(NULL, NULL, CLEAN);
	node->type = NODE_PIPE;
	node->fd_in = STDIN_FILENO;
	node->fd_out = STDOUT_FILENO;
	return (node);
}

//void	print_node()

void	append_node(t_ast **head, t_ast *new_node)
{
	t_ast	*tmp;

	if (!new_node)
		return ;
	if (*head)
	{
		tmp = *head;
		while (tmp->right)
			tmp = tmp->right;
		tmp->right = new_node;
		new_node->left = tmp;
	}
	else
		*head = new_node;
}

t_ast	*parse_tokens(t_token **tokens)
{
	t_ast	*head;
	t_ast	*current_node;
	t_ast	*temp_left;
	t_ast	*temp_right;
	char	**args;
	int		count;
	int		capacity = INITIAL_TOKEN_CAPACITY;
	int		i;

	i = 0;
	head = NULL;
	if (!tokens)
		return (NULL);
	args = ft_calloc(capacity + 1, sizeof(t_token));
	if (!args)
	{
		perror("calloc in parse tokens");
		lst_memory(NULL, NULL, CLEAN);
	}
	while (tokens[i])
	{
		if (tokens[i]->token_type == TOKEN_WORD)
		{
			while (tokens[i]->token_type == TOKEN_WORD)
			{
				args[count++] = tokens[i]->token_value;
				i++;
			}
			current_node = create_command_node(args);
			append_node(&head, current_node);
		}
		else if (tokens[i]->token_type == TOKEN_AND
				|| tokens[i]->token_type == TOKEN_OR)
		{
			i++;
			current_node = create_pipe_node();
			append_node(&head, current_node);
		}
		else if (tokens[i]->token_type == TOKEN_REDIRECT_IN
				|| tokens[i]->token_type == TOKEN_REDIRECT_OUT
				|| tokens[i]->token_type == TOKEN_REDIRECT_APPEND)
		{
			i++; // because i want to skip special symbol?
			current_node = create_redireciton_node(tokens[i]->token_value);
			append_node(&head, current_node);
		}
		else if (tokens[i]->token_type == TOKEN_PIPE)
		{
			i++;
			current_node = create_pipe_node();
			append_node(&head, current_node);
		}
	}
}
