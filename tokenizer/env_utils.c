/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 19:45:29 by anarama           #+#    #+#             */
/*   Updated: 2024/07/20 19:45:55 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	copy_with_new_env_var(char *new_input, const char *original_input, char *env_string, int initial_var_len)
{
	int i;
	int j;

	i = 0;
	while (original_input[i] != '$')
	{
		new_input[i] = original_input[i];
		i++;
	}
	j = i + initial_var_len + 1;
	if (!env_string)
		return ;
	while (*env_string)
	{
		new_input[i] = *env_string;
		env_string++;
		i++;
	}
	while (original_input[j] && original_input[j] != '"')
	{
		new_input[i] = original_input[j];
		i++;
		j++;
	}
}

int get_len_next_double_quote(const char *input, char **env, char **new_input)
{
    int i = 0;
    int second_double_found = 0;
	int initial_var_len;
	char *env_string;

	env_string = NULL;
    while (input[i] && !ft_is_double_quote(input[i], &second_double_found))
	{
		if (input[i] == '$')
		{
			initial_var_len = get_len_next_space_or_quote(input + i + 1);
			env_string = ft_substr(input + i + 1, 0, initial_var_len);
			if (!env_string)
			{
				perror("substr failed while expanding env var in quotes");
				lst_memory(NULL, NULL, CLEAN);
			}
            handle_dollar_sign(&env_string, env);
			*new_input = ft_calloc(ft_strlen(input) - initial_var_len + 1 + ft_strlen(env_string) + 1, sizeof(char));
			if (!*new_input)
			{
				perror("substr failed while expanding env var in quotes");
				lst_memory(NULL, NULL, CLEAN);
			}
			copy_with_new_env_var(*new_input, input, env_string, initial_var_len);
		}
		i++;
	}
    if (second_double_found == 0)
	{
		return (0);
	}
    return (i + 1);
}