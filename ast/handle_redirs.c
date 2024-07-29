/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrejarama <andrejarama@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 11:56:47 by anarama           #+#    #+#             */
/*   Updated: 2024/07/28 15:40:32 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>

char	**cat_args(char **left, char **right)
{
	char	**new_arr;
	int		len_left;
	int		len_right;
	int		i;

	len_left = get_split_size((const char **)left);
	len_right = get_split_size((const char **)right);
	new_arr = ft_calloc(len_left + len_right + 1, sizeof(t_token *));
	lst_memory(new_arr, free_split, ADD);
	i = 0;
	while (i < len_left)
	{
		new_arr[i] = ft_strdup(left[i]);
		i++;
	}
	i = 0;
	while (i < len_right)
	{
		new_arr[i + len_left] = ft_strdup(right[i]);
		i++;
	}
	return (new_arr);
}

void	check_valid_redir(t_ast *redir_node, int *error_catched)
{
	if (!redir_node->file)
	{
		printf("minishell: syntax error near unexpected token 'newline'\n");
		*error_catched = 1;
	}
	else if (is_double_special(redir_node->file) || is_single_special(*redir_node->file))
	{
		printf("minishell: syntax error near unexpected token '%s'\n", redir_node->file);
		*error_catched = 1;
	}
}

int32_t	setup_flags_and_fds(t_ast *redir_node)
{
	if (redir_node->token_type == TOKEN_REDIRECT_IN)
	{
		return (O_WRONLY | O_CREAT | O_TRUNC);
	}
	else if (redir_node->token_type == TOKEN_REDIRECT_OUT)
	{
		return  (O_RDONLY);
	}
	else if (redir_node->token_type == TOKEN_REDIRECT_APPEND)
	{
		return (O_WRONLY | O_CREAT | O_APPEND);
	}
	else
		return (0);
}

void	handle_redir_out(t_ast *save_ptr_left, t_ast *redir_node, int *error_catched)
{
	if (save_ptr_left->fd_file_in)
	{
		ft_close(save_ptr_left->fd_file_in, "redir");
		save_ptr_left->fd_file_in = 0;
	}
	ft_open(&save_ptr_left->fd_file_in, redir_node->file,
		save_ptr_left->flags, 0644);
	if (save_ptr_left->fd_file_in == -1)
	{
		*error_catched = 1;
		return ;
	}
	save_ptr_left->file = redir_node->file;
	if (redir_node->right && redir_node->right->args && !redir_node->right->is_done)
	{
		save_ptr_left->args = cat_args(save_ptr_left->args, redir_node->right->args);
		redir_node->right->is_done = 1;
	}
}

void	handle_redir_in(t_ast *save_ptr_left, t_ast *redir_node, int *error_catched)
{
	if (save_ptr_left->fd_file_out > 1)
	{
		ft_close(save_ptr_left->fd_file_out, "redir");
		save_ptr_left->fd_out = 0;
	}
	ft_open(&save_ptr_left->fd_file_out, redir_node->file,
		save_ptr_left->flags, 0644);
	if (save_ptr_left->fd_file_out == 1)
		printf("fuck this shit");
	if (save_ptr_left->fd_file_out == -1)
	{
		*error_catched = 1;
		return ;
	}
	save_ptr_left->file = redir_node->file;
	if (redir_node->right && redir_node->right->args && !redir_node->right->is_done)
	{
		save_ptr_left->args = cat_args(save_ptr_left->args, redir_node->right->args);
		redir_node->right->is_done = 1;
	}
}

void	setup_left_command_node(t_ast *redir_node, t_ast **head)
{
	if (redir_node->left == NULL)
	{
		t_ast *temp = create_command_node(TOKEN_WORD, NULL);
		lst_memory(temp, free, ADD);
		temp->right = redir_node;
		redir_node->left = temp;
		*head = temp;
	}
	while (redir_node->left)
	{
		if (redir_node->left->type == NODE_COMMAND && !redir_node->left->is_done)
			break ;
		redir_node->left = redir_node->left->left;
	}
}

void	handle_redir(t_ast *redir_node, t_ast **head, int *error_catched)
{
	int32_t	flags;
	int32_t	fd;

	check_valid_redir(redir_node, error_catched);
	if (*error_catched)
		return ;
	flags = setup_flags_and_fds(redir_node);
	if (redir_node->token_type == TOKEN_REDIRECT_IN || redir_node->token_type == TOKEN_REDIRECT_APPEND)
	{
		ft_open(&fd, redir_node->file, flags, 0644);
		ft_dup2(fd, STDOUT_FILENO, "redir");
		ft_close(fd, "redir");
	}
	else if (redir_node->token_type == TOKEN_REDIRECT_OUT)
	{
		ft_open(&fd, redir_node->file, flags, 0644);
		ft_dup2(fd, STDIN_FILENO, "redir");
		ft_close(fd, "redir");
	}
	redir_node->is_done = 1;
	if (*error_catched)
		return ;
}
