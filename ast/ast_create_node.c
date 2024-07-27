/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_create_node.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 17:45:01 by anarama           #+#    #+#             */
/*   Updated: 2024/07/27 21:07:19 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_ast	*create_command_node(t_token_type token_type, char **args)
{
	t_ast	*node;

	node = ft_calloc(1, (sizeof(t_ast)));
	if (!node)
		lst_memory(NULL, NULL, CLEAN);
	node->type = NODE_COMMAND;
	node->token_type = token_type;
	if (args)
		node->args = copy_args(node, args);
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
