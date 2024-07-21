/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_token_env_var.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 12:37:49 by anarama           #+#    #+#             */
/*   Updated: 2024/07/21 16:48:16 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_env_var(const char *input)
{
	return (*input == '$');
}

t_token	*create_token_env_var(const char **input, const char **env)
{
	t_token	*temp_token;
	int		temp;
	char	*temp_str;

	temp_token = NULL;
	if (**input == '$')
	{
		temp = get_len_next_space(*input + 1);
		temp_str = ft_substr(*input + 1, 0, temp);
		if (!temp_str)
		{
			perror("substr failed while expanding env var");
			lst_memory(NULL, NULL, CLEAN);
		}
		lst_memory(temp_str, free, ADD);
		handle_dollar_sign(&temp_str, (char **)env);
		temp_token = create_token(TOKEN_ENV, temp_str);
		*input += temp + 1;
	}
	return (temp_token);
}
