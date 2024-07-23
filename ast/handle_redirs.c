/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 11:56:47 by anarama           #+#    #+#             */
/*   Updated: 2024/07/23 12:06:16 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**cat_args(char **left, char **right)
{
	char	**new_arr;
	int		len_left;
	int		len_right;
	int		i;

	len_left = get_tokens_len((t_token **)left);
	len_right = get_tokens_len((t_token **)right);
	new_arr = ft_calloc(len_left + len_right + 1, sizeof(t_token *));
	lst_memory(new_arr, free_split, ADD);
	i = 0;
	while (i < len_left)
	{
		new_arr[i] = ft_strdup(left[i]);
		// not sure
		i++;
	}
	i = 0;
	while (i < len_right)
	{
		new_arr[i + len_left] = ft_strdup(right[i]);
		i++;
	}
	//free_split(left);
	return (new_arr);
}

void	check_valid_redir(t_ast *redir_node)
{
	if (!redir_node->file)
	{
		printf("minishell: syntax error near unexpected token 'newline'\n");
		lst_memory(NULL, NULL, CLEAN);
	}
	else if (is_double_special(redir_node->file) || is_single_special(redir_node->file))
	{
		printf("minishell: syntax error near unexpected token '%s'\n", redir_node->file);
		lst_memory(NULL, NULL, CLEAN);
	}
}

void	setup_flags_and_fds(t_ast *redir_node)
{
	if (redir_node->token_type == TOKEN_REDIRECT_IN)
	{
		redir_node->left->flags = O_WRONLY | O_CREAT | O_TRUNC;
		redir_node->left->std_fd = STDOUT_FILENO;
	}
	else if (redir_node->token_type == TOKEN_REDIRECT_OUT)
	{
		redir_node->left->flags = O_RDONLY;
		redir_node->left->std_fd = STDIN_FILENO;
	}
	else if (redir_node->token_type == TOKEN_REDIRECT_APPEND)
	{
		redir_node->left->flags = O_WRONLY | O_CREAT | O_APPEND;
		redir_node->left->std_fd = STDOUT_FILENO;
	}
}
//repoint head to te beginnin?

void	handle_redir(t_ast *redir_node)
{

	check_valid_redir(redir_node);
	if (redir_node->left == NULL)
	{
		redir_node->left = create_command_node(TOKEN_WORD, NULL);
	}
	setup_flags_and_fds(redir_node);
	redir_node->left->file = redir_node->file;
	if (redir_node->right && redir_node->right->args)
	{
		redir_node->left->args = cat_args(redir_node->left->args, redir_node->right->args);
		redir_node->right->is_done = 1;
	}
	redir_node->is_done = 1;
	// ft_open(&redir_node->left->fd_in, redir_node->file, flags, 0644);
	// ft_dup2(redir_node->left->fd_in, std_fd, "dup2 redirections");
	// ft_close(redir_node->left->fd_in, "close redirections");
}
