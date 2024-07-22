/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_token_env_var.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 12:37:49 by anarama           #+#    #+#             */
/*   Updated: 2024/07/22 13:47:16 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_env_var(const char *input)
{
	return (*input == '$');
}

t_token	create_token_env_var(char **input, const char **environement)
{
	t_token	temp_token;
	uint32_t	i;
	char	*temp_move;
	char	*temp_str;

	i = 0;
	temp_move = ft_strchr(*input, ' ');
	temp_str = extract_variable(*input, environement);
	temp_token = create_token(TOKEN_ENV, temp_str);
	if (temp_move)
	{
		*temp_move = 0;
		*input = temp_move + 1;
	}
	else
		*input = ft_strchr(*input, 0);
	return (temp_token);
}
