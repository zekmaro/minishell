/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 11:56:47 by anarama           #+#    #+#             */
/*   Updated: 2024/07/27 20:14:06 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

void	check_valid_redir(t_ast *redir_node)
{
	if (!redir_node->file)
	{
		printf("minishell: syntax error near unexpected token 'newline'\n");
	}
	else if (is_double_special(redir_node->file) || is_single_special(*redir_node->file))
	{
		printf("minishell: syntax error near unexpected token '%s'\n", redir_node->file);
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

void	handle_redir(t_ast *redir_node, t_ast **head)
{
	check_valid_redir(redir_node);
	if (redir_node->left == NULL)
	{
		t_ast *temp = create_command_node(TOKEN_WORD, NULL);
		lst_memory(temp, free, ADD);
		temp->right = redir_node;
		redir_node->left = temp;
		*head = temp;
	}
	setup_flags_and_fds(redir_node);
	redir_node->left->file = redir_node->file;
	/*print_ast(*head);*/
	if (redir_node->right && redir_node->right->args)
	{
		redir_node->left->args = cat_args(redir_node->left->args, redir_node->right->args);
		redir_node->right->is_done = 1;
	}
	redir_node->is_done = 1;
}
