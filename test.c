/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 10:58:26 by anarama           #+#    #+#             */
/*   Updated: 2024/07/17 17:38:52 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **env)
{
	char		*input = argv[1];
	t_token		**tokens;
	t_ast		*ast;

	ft_printf("%s\n", argv[1]);
	tokens = lexical_analysis(input, env);
	printf("---TOKENS---\n");
	print_tokens(tokens);
	printf("------------\n");
	
}