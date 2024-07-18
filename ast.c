/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 13:17:47 by anarama           #+#    #+#             */
/*   Updated: 2024/07/18 17:09:20 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//cleaning mmeory like this wont work in the proccess i might fail
// if one of allocation failed clean the whole list is cleaned but if all success i put head in lstmem
// add each node in the lst mem?

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
	temp = dest;
	while (*src)
	{
		*dest = ft_strdup(*src);
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
		else {
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

void	ft_free_split(char **args)
{
	char **temp;

	temp = args;
	while (*args)
	{
		free(*args);
		*args = NULL;
	}
	free(temp);
}

//put protection inside the fucntions. free and exit inside
// doubling capacity can be out of INT boundaries: do long / size_t or somethign?

t_ast	*parse_tokens(t_token **tokens)
{
	t_ast	*head;
	t_ast	*current_node;
	char	**args;
	int		count;
	int		capacity = INITIAL_TOKEN_CAPACITY;
	int		len_tokens;
	int		i;

	i = 0;
	count = 0;
	head = NULL;
	len_tokens = get_tokens_len(tokens);
	if (!tokens)
		return (NULL);
	while (tokens[i])
	{
		printf("%s %d ", tokens[i]->token_value, tokens[i]->token_type);
		if (tokens[i]->token_type == TOKEN_WORD)
		{
			printf("word\n");
			args = ft_calloc(capacity + 1, sizeof(t_token));
			if (!args)
			{
				perror("calloc in parse tokens");
				lst_memory(NULL, NULL, CLEAN);
			}
			while (tokens[i] && tokens[i]->token_type == TOKEN_WORD)
			{
				args[count] = tokens[i]->token_value;
				if (count >= capacity)
				{
					args = (char **)custom_realloc((void **)args, capacity, capacity * 2);
					if (!args)
					{
						perror("calloc in parse tokens");
						lst_memory(NULL, NULL, CLEAN);
					}
					capacity *= 2;
				}
				count++;
				i++;
			}
			i--;
			current_node = create_command_node(tokens[i]->token_type, args);
			ft_free_split(args);
			count = 0;
			append_node(&head, current_node);
		}
		else if (tokens[i]->token_type == TOKEN_AND
				|| tokens[i]->token_type == TOKEN_OR)
		{
			printf("logic\n");
			current_node = create_logical_node(tokens[i]->token_type);
			append_node(&head, current_node);
		}
		else if (tokens[i]->token_type == TOKEN_REDIRECT_IN
				|| tokens[i]->token_type == TOKEN_REDIRECT_OUT
				|| tokens[i]->token_type == TOKEN_REDIRECT_APPEND
				|| tokens[i]->token_type == TOKEN_HEREDOC)
		{
			printf("redirection\n");
			if (i + 1 < len_tokens)
			{
				i++;
				current_node = create_redireciton_node(tokens[i - 1]->token_type, tokens[i]->token_value);
			}
			append_node(&head, current_node);
		}
		else if (tokens[i]->token_type == TOKEN_PIPE)
		{
			printf("pipe\n");
			current_node = create_pipe_node(tokens[i]->token_type);
			append_node(&head, current_node);
		}
		current_node = NULL;
		i++;
	}
	return (head);
}
