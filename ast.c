/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 13:17:47 by anarama           #+#    #+#             */
/*   Updated: 2024/07/20 16:44:23 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_ast(t_ast *head)
{
	int i;

	i = 0;
	while (head)
	{
		printf("NODE %d\n", i);
		if (head->type == NODE_COMMAND)
			printf("TYPE: COMMAND\n");
		else if (head->type == NODE_REDIRECTION)
			printf("TYPE: REDIRECTION\n");
		else if (head->type == NODE_PIPE)
			printf("TYPE: PIPE\n");
		else if (head->type == NODE_LOGICAL_OPERATOR)
			printf("TYPE: LOGICAL_OPERATOR\n");
		if (head->token_type == TOKEN_REDIRECT_IN)
			printf("TOKEN TYPE: REDIRECT_IN\n");
		else if (head->token_type == TOKEN_REDIRECT_OUT)
			printf("TOKEN TYPE: REDIRECT_OUT\n");
		else if (head->token_type == TOKEN_REDIRECT_APPEND)
			printf("TOKEN TYPE: REDIRECT_APPEND\n");
		else if (head->token_type == TOKEN_HEREDOC)
			printf("TOKEN TYPE: HEREDOC\n");
		else if (head->token_type == TOKEN_AND)
			printf("TOKEN TYPE: AND\n");
		else if (head->token_type == TOKEN_OR)
			printf("TOKEN TYPE: OR\n");
		printf("ARGS: ");
		if (head->args && *head->args)
		{
			while (*head->args)
			{
				printf("[%s]", *head->args);
				head->args++;
			}
		}
		else 
		{
			printf("NULL");
		}
		printf("\n");
		printf("LEFT: %p\n", head->left);
		printf("RIGHT: %p\n", head->right);
		printf("FILE: %s\n", head->file);
		printf("FD_IN: %d\n", head->fd_in);
		printf("FD_OUT: %d\n", head->fd_out);
		head = head->right;
		printf("\n");
		i++;
	}
}

int	get_tokens_len(t_token **tokens)
{
	int i;

	i = 0;
	while (*tokens)
	{
		i++;
		tokens++;
	}
	return (i);
}

char	**copy_args(char **dest, char **src)
{
	int i;
	char **temp;

	i = get_tokens_len((t_token **)src);
	dest = ft_calloc(i + 1, sizeof(char *));
	if (!dest)
	{
		perror("calloc in copy args");
		lst_memory(NULL, NULL, CLEAN);
	}
	lst_memory(dest, free_split, ADD);
	temp = dest;
	while (*src)
	{
		*dest = ft_strdup(*src);
		if (!*dest)
		{
			perror("strdup in copy args");
			lst_memory(NULL, NULL, CLEAN);
		}
		src++;
		dest++;
	}
	return (temp);
}

t_ast	*create_command_node(t_token_type token_type, char **args)
{
	t_ast	*node;

	node = ft_calloc(1, (sizeof(t_ast)));
	if (!node)
		lst_memory(NULL, NULL, CLEAN);
	node->type = NODE_COMMAND;
	node->token_type = token_type;
	if (args)
		node->args = copy_args(node->args, args);
	node->fd_in = STDIN_FILENO;
	node->fd_out = STDOUT_FILENO;
	return (node);
}

t_ast	*create_redireciton_node(t_token_type token_type, char *file_name)
{
	t_ast	*node;

	node = ft_calloc(1, (sizeof(t_ast)));
	if (!node)
		lst_memory(NULL, NULL, CLEAN);
	node->type = NODE_REDIRECTION;
	node->token_type = token_type;
	node->file = file_name;
	node->fd_in = STDIN_FILENO;
	node->fd_out = STDOUT_FILENO;
	return (node);
}

t_ast	*create_pipe_node(t_token_type token_type)
{
	t_ast	*node;

	node = ft_calloc(1, (sizeof(t_ast)));
	if (!node)
		lst_memory(NULL, NULL, CLEAN);
	node->type = NODE_PIPE;
	node->token_type = token_type;
	node->fd_in = STDIN_FILENO;
	node->fd_out = STDOUT_FILENO;
	return (node);
}

t_ast	*create_logical_node(t_token_type token_type)
{
	t_ast	*node;

	node = ft_calloc(1, (sizeof(t_ast)));
	if (!node)
		lst_memory(NULL, NULL, CLEAN);
	node->type = NODE_LOGICAL_OPERATOR;
	node->token_type = token_type;
	node->fd_in = STDIN_FILENO;
	node->fd_out = STDOUT_FILENO;
	return (node);
}

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
		new_node->left = tmp;
		tmp->right = new_node;
	}
	else
		*head = new_node;
}

void	clear_ast(void *head)
{
	t_ast *copy_head;
	t_ast *temp;

	temp = NULL;
	copy_head = (t_ast *)head;
	while (copy_head)
	{
		temp = copy_head;
		copy_head = copy_head->right;
		free(temp);
	}
}

void	parse_word(t_ast **head, int *i, t_token **tokens)
{
	char	**args;
	int		capacity = INITIAL_TOKEN_CAPACITY;
	int		count;
	t_ast	*current_node;

	count = 0;
	current_node = NULL;
	args = ft_calloc(capacity + 1, sizeof(char *));
	if (!args)
	{
		perror("calloc in parse tokens");
		lst_memory(NULL, NULL, CLEAN);
	}
	lst_memory(args, free_split, ADD);
	while (tokens[*i] && tokens[*i]->token_type == TOKEN_WORD)
	{
		args[count] = ft_strdup(tokens[*i]->token_value);
		if (!args[count])
		{
			perror("strdup fopr token value in parsing");
			lst_memory(NULL, NULL, CLEAN);
		}
		if (count >= capacity)
		{
			args = (char **)custom_realloc((void **)args, capacity, capacity * 2, 0);
			if (!args)
			{
				perror("calloc in parse tokens");
				lst_memory(NULL, NULL, CLEAN);
			}
			capacity *= 2;
		}
		count++;
		(*i)++;
	}
	(*i)--;
	current_node = create_command_node(tokens[*i]->token_type, args);
	append_node(head, current_node);
	count = 0;
}

void	parse_logical_operator(t_ast **head, int *i, t_token **tokens)
{
	t_ast	*current_node;

	current_node = NULL;
	current_node = create_logical_node(tokens[*i]->token_type);
	append_node(head, current_node);
}

void	parse_redirection(t_ast **head, int *i, t_token **tokens)
{
	int		len_tokens;
	t_ast	*current_node;

	current_node = NULL;
	len_tokens = get_tokens_len(tokens);
	if (*i + 1 < len_tokens)
	{
		(*i)++;
		current_node = create_redireciton_node(tokens[*i - 1]->token_type, tokens[*i]->token_value);
	}
	append_node(head, current_node);
}

int	is_redirection(t_token_type	token_type)
{
	return (token_type == TOKEN_REDIRECT_IN
		|| token_type == TOKEN_REDIRECT_OUT
		|| token_type == TOKEN_REDIRECT_APPEND
		|| token_type == TOKEN_HEREDOC);
}

void	parse_pipe(t_ast **head, int *i, t_token **tokens)
{
	t_ast	*current_node;

	current_node = NULL;
	current_node = create_pipe_node(tokens[*i]->token_type);
	append_node(head, current_node);
}

t_ast	*parse_tokens(t_token **tokens)
{
	t_ast	*head;
	int		i;
	int		in_lst;

	if (!tokens)
		return (NULL);
	i = 0;
	in_lst = 0;
	head = NULL;
	while (tokens[i])
	{
		if (tokens[i]->token_type == TOKEN_WORD)
		{
			parse_word(&head, &i, tokens);
		}
		else if (tokens[i]->token_type == TOKEN_AND
				|| tokens[i]->token_type == TOKEN_OR)
		{
			parse_logical_operator(&head, &i, tokens);
		}
		else if (is_redirection(tokens[i]->token_type))
		{
			parse_redirection(&head, &i, tokens);
		}
		else if (tokens[i]->token_type == TOKEN_PIPE)
		{
			parse_pipe(&head, &i, tokens);
		}
		if (head != NULL && !in_lst)
		{
			lst_memory((void *)head, clear_ast, ADD);
			in_lst = 1;
		}
		i++;
	}
	return (head);
}
