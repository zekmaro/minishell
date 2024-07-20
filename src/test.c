/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 10:58:26 by anarama           #+#    #+#             */
/*   Updated: 2024/07/20 18:49:48 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// void	restore_fd(int original_stdin, int original_stdout)
// {
// 	dup2(original_stdin, STDIN_FILENO);
// 	dup2(original_stdout, STDOUT_FILENO);
// 	close(original_stdin);
// 	close(original_stdout);
// }

// char	**cat_args(char **dest, char **src)
// {
// 	int		len_dest;
// 	int		len_src;
// 	char	**new_arr;
// 	int		i;

// 	len_dest = get_tokens_len((t_token **)dest);
// 	len_src = get_tokens_len((t_token **)src);
// 	new_arr = ft_calloc(len_dest + len_src + 1, sizeof(t_token *));
// 	i = 0;
// 	while (i < len_dest)
// 	{
// 		new_arr[i] = ft_strdup(dest[i]);
// 		i++;
// 	}
// 	i = 0;
// 	while (i < len_src)
// 	{
// 		new_arr[i + len_dest] = ft_strdup(src[i]);
// 		i++;
// 	}
// 	ft_free_split(dest);
// 	ft_free_split(src);
// 	return (new_arr);
// }

// void	parse_redirection(t_ast **ast)
// {
// 	int	flags;
// 	int std_fd;

// 	if (!(*ast)->right)
// 	{
// 		printf("minishell: syntax error near unexpected token 'newline'\n");
// 		lst_memory(NULL, NULL, CLEAN);
// 	}
// 	else if ((*ast)->right->type != NODE_COMMAND)
// 	{
// 		if ((*ast)->right->token_type == TOKEN_OR)
// 			printf("minishell: syntax error near unexpected token '%s'\n", "||");
// 		else if ((*ast)->right->token_type == TOKEN_AND)
// 			printf("minishell: syntax error near unexpected token '%s'\n", "&&");
// 		else if ((*ast)->right->token_type == TOKEN_REDIRECT_APPEND)
// 			printf("minishell: syntax error near unexpected token '%s'\n", ">>");
// 		else if ((*ast)->right->token_type == TOKEN_HEREDOC)
// 			printf("minishell: syntax error near unexpected token '%s'\n", "<<");
// 		else if ((*ast)->right->token_type == TOKEN_PIPE)
// 			printf("minishell: syntax error near unexpected token '%s'\n", "|");
// 		else if ((*ast)->right->token_type == TOKEN_REDIRECT_IN)
// 			printf("minishell: syntax error near unexpected token '%s'\n", ">");
// 		else if ((*ast)->right->token_type == TOKEN_REDIRECT_OUT)
// 			printf("minishell: syntax error near unexpected token '%s'\n", "<");
// 		lst_memory(NULL, NULL, CLEAN);
// 	}
// 	if ((*ast)->left == NULL)
// 	{
// 		(*ast)->left = create_command_node(TOKEN_WORD, NULL);
// 	}
// 	if ((*ast)->right->token_type == TOKEN_REDIRECT_IN)
// 	{
// 		flags = O_WRONLY | O_CREAT | O_TRUNC;
// 		std_fd = STDOUT_FILENO;
// 	}
// 	else if ((*ast)->right->token_type == TOKEN_REDIRECT_OUT)
// 	{
// 		flags = O_RDONLY;
// 		std_fd = STDIN_FILENO;
// 	}
// 	else if ((*ast)->right->token_type == TOKEN_REDIRECT_APPEND)
// 	{
// 		flags = O_WRONLY | O_CREAT | O_APPEND;
// 		std_fd = STDOUT_FILENO;
// 	}
// 	if ((*ast)->right->args)
// 		(*ast)->left->args = cat_args((*ast)->left->args, (*ast)->right->args);
// 	ft_open((&(*ast)->left->fd_in), (*ast)->right->file, flags, 0644);
// 	ft_dup2((*ast)->left->fd_in, std_fd, "dup2 redirections");
// 	ft_close((*ast)->left->fd_in, "close redirections");
// }

// void	parse_pipe(t_ast **ast)
// {

// }

// void	parse_logical_operator(t_ast **ast)
// {
	
// }

int	main(int argc, char **argv, char **env)
{
	char		*input = argv[1];
	t_token		**tokens;
	t_ast		*ast;
	// int	original_stdin = dup(STDIN_FILENO);
	// int	original_stdout = dup(STDOUT_FILENO);

	ft_printf("%s\n", argv[1]);
	tokens = lexical_analysis(input, env);
	printf("---TOKENS---\n");
	print_tokens(tokens);
	printf("------------\n");
	ast = parse_tokens(tokens);
	printf("----AST----\n");
	print_ast(ast);
	printf("-----------\n");
	lst_memory(NULL, NULL, CLEAN);
	// while (ast)
	// {
	// 	if (ast->type == NODE_REDIRECTION)
	// 	{

	// 	}
	// 	else if (ast->type == NODE_PIPE)
	// 	{
			
	// 	}
	// 	else if (ast->type == NODE_LOGICAL_OPERATOR)
	// 	{
			
	// 	}
	// 	ast = ast->right;
	// }
}

// #1 EXPAND ENV VARS INSIDE QUOTES
// #2 FIX LEAKS AND MANAGE MEMORY PROPERLY