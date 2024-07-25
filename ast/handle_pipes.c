/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 20:49:34 by andrejarama       #+#    #+#             */
/*   Updated: 2024/07/25 12:47:23 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void handle_pipe(t_ast *pipe_node)
{
    int pipe_fd[2];

    if (pipe(pipe_fd) == -1)
    {
        perror("pipe");
        lst_memory(NULL, NULL, CLEAN);
    }
	if (pipe_node->left && pipe_node->right)
    {
		while (pipe_node->left && pipe_node->left->token_type != NODE_COMMAND)
		{
			pipe_node->left = pipe_node->left->left;
		}
        pipe_node->left->fd_out = pipe_fd[1];
		while (pipe_node->right && pipe_node->right->token_type != NODE_COMMAND)
		{
			pipe_node->right = pipe_node->right->right;
		}
		pipe_node->right->fd_in = pipe_fd[0];
    }
	else
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
    pipe_node->is_done = 1;
}
