/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_print.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 17:43:48 by anarama           #+#    #+#             */
/*   Updated: 2024/07/20 17:53:29 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_ast(t_ast *head)
{
	int	i;

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
