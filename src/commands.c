/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrejarama <andrejarama@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 21:20:49 by victor            #+#    #+#             */
/*   Updated: 2024/07/30 12:36:18 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>

void	restore_fd(int original_stdin, int original_stdout)
{
	dup2(original_stdin, STDIN_FILENO);
	dup2(original_stdout, STDOUT_FILENO);
}

void	command_execute(const char *command_path,
						const char **argv,
						const char **env)
{
	pid_t	child_proccess;

	ft_fork(&child_proccess, command_path);
	if (child_proccess == 0)
	{
		execve(command_path, (char **)argv, (char **)env);
		perror("execve");
		lst_memory(NULL, NULL, CLEAN);
	}
	else
	{
		waitpid(child_proccess, NULL, 0);
	}
}

void	execute_commands(t_ast *tree, const char *path_variable,
					const char **env, int *error_catched)
{
	static int	exit_status;
	t_tree_connection_type connection_type;
	uint32_t	i;

	exit_status = *error_catched;
	i = 0;
	while (tree[i].type != NODE_END)
	{
		handle_command(&tree[i], path_variable, env, &exit_status);
		if (tree[i].connection_type == TREE_LOGICAL_OR && exit_status == 0)
			i++;
		else if (tree[i].connection_type == TREE_LOGICAL_AND && exit_status != 0)
			i++;
		i++;
	}
}

void	check_valid_logical_operator(t_ast *logical_node, int *error_catched)
{
	if (!logical_node->left || !logical_node->right)
	{
		printf("minishell: syntax error near unexpected token `&&'\n");
		*error_catched = 1;
	}
}
/**/
/*void	traverse_tree(t_ast	*ast, t_ast **head, int *error_catched)*/
/*{*/
/*	while (ast)*/
/*	{*/
/*		if (ast->type == NODE_REDIRECTION)*/
/*		{*/
/*			handle_redir(ast, head, error_catched);*/
/*		}*/
/*		else if (ast->type == NODE_LOGICAL_OPERATOR)*/
/*		{*/
/*			check_valid_logical_operator(ast, error_catched);*/
/*		}*/
/*		if (*error_catched)*/
/*			return ;*/
/*		ast = ast->right;*/
/*	}*/
/*}*/

void	print_tokens(t_token *tokens)
{
	int i;

	i = 0;
	printf("----TOKENS----\n");
	while (tokens[i].token_type != TOKEN_EOL)
	{
		printf("Token: Type=%d, Value=%s\n", tokens[i].token_type, tokens[i].token_value);
		i++;
	}
	printf("------------\n");
}

void	skip_up_to_logical_operator(t_ast *ast)
{
	while (ast)
	{
		if (ast->type == NODE_LOGICAL_OPERATOR)
			break ;
		ast->is_done = 1;
		ast = ast->right;
	}
}

void	*m_tokenizer(const char *input, const char **env,
			const char *path_variable)
{
	t_token	*tokens;
	t_ast	*tree;
	int		original_stdin;
	int		original_stdout;
	int	error_catched;

	error_catched = 0;
	original_stdin = dup(STDIN_FILENO);
	original_stdout = dup(STDOUT_FILENO);
	lst_memory((void *)input, free, ADD);
	tokens = lexical_analysis(input, env);
	tree = parse_tokens(tokens);
	if (error_catched)
		skip_up_to_logical_operator(tree);
	execute_commands(tree, path_variable, env, &error_catched);
	lst_memory(tokens, NULL, FREE);
	restore_fd(original_stdin, original_stdout);
	return (NULL);
}
