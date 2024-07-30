/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 20:49:34 by andrejarama       #+#    #+#             */
/*   Updated: 2024/07/28 00:44:18 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>

void handle_pipe(t_ast *pipe_node)
{
    int pipe_fd[2];

    if (pipe(pipe_fd) == -1)
    {
        perror("pipe");
        lst_memory(NULL, NULL, CLEAN);
    }
    if (pipe_node->left && pipe_node->left->type == NODE_COMMAND)
    {
        pipe_node->left->fd_out = pipe_fd[1];
    }
    if (pipe_node->right && pipe_node->right->type == NODE_COMMAND)
    {
        pipe_node->right->fd_in = pipe_fd[0];
    }
    pipe_node->is_done = 1;
}
