/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_token_env_var.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 12:37:49 by anarama           #+#    #+#             */
/*   Updated: 2024/07/23 17:07:00 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_env_var(const char *input)
{
	return (*input == '$');
}

t_token	create_token_env_var(char **input, const char **environement)
{
	char		**variable_pointers;
	char		*buffer;
	uint32_t	variable_count;
	char		*temp_move;
	char		*temp_str;

	variable_count = determine_variables(*input);
	variable_pointers = ft_calloc(variable_count + 1, sizeof(char *));
	temp_move = ft_strchr(*input, ' ');
	if (temp_move)
		*temp_move = 0;
	extract_variable(variable_pointers, *input, environement, variable_count);
	buffer = extract_word((char *)*input, variable_pointers);
	if (temp_move)
		*input = temp_move + 1;
	else
		*input = ft_strchr(*input, 0);
	return (create_token(TOKEN_ENV, buffer));
}
