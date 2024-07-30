/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 16:56:11 by vvobis            #+#    #+#             */
/*   Updated: 2024/07/26 16:14:24 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

uint32_t	determine_variables(const char *command_input)
{
	char		*tmp;
	uint32_t	i;

	tmp = ft_strchr(command_input, '$');
	i = 0;
	while (tmp)
	{
		tmp = ft_strchr(tmp + 1, '$');
		i++;
	}
	return (i);
}

void	extract_word(	char **buffer, \
						char *command_input, \
						char **variable_pointers)
{
	uint32_t	i;
	uint32_t	j;
	char		*tmp;

	*buffer = ft_calloc(get_split_length(variable_pointers) \
						+ ft_strlen(command_input) + 1, sizeof(*buffer));
	if (!*buffer)
		lst_memory(NULL, NULL, CLEAN);
	tmp = *buffer;
	j = 0;
	while (*command_input || variable_pointers[j])
	{
		i = 0;
		while (*command_input && *command_input != '$')
			*(*buffer)++ = *command_input++;
		if (*command_input)
			command_input++;
		while (variable_pointers[j] && variable_pointers[j][i])
			*(*buffer)++ = variable_pointers[j][i++];
		while (*command_input && *command_input != ' ' && *command_input != '$')
			command_input++;
		if (variable_pointers[j])
			j++;
	}
	*buffer = tmp;
}

void	extract_variable(char **variable_pointers, \
						const char *command_input, \
						const char **environement, \
						uint32_t variable_count)
{
	char		*tmp;
	char		store_temp_eol;
	uint32_t	i;
	uint32_t	j;

	j = 0;
	tmp = (char *)command_input;
	while (j < variable_count)
	{
		i = 0;
		tmp = ft_strchr(tmp, '$');
		if (!tmp)
			return ;
		tmp++;
		while (tmp[i] && tmp[i] != '$' && tmp[i] != ' ')
			i++;
		store_temp_eol = tmp[i];
		tmp[i] = 0;
		variable_pointers[j] = environment_variable_value_get(tmp, \
														environement);
		tmp[i] = store_temp_eol;
		j++;
	}
}

char	*interpret_single_quote(const char *command_input)
{
	char	*tmp;

	tmp = ft_strchr(command_input + 1, '\'');
	if (!tmp)
		lst_memory(NULL, NULL, CLEAN);
	*tmp = 0;
	tmp = (char *)command_input + 1;
	command_input = ft_strchr(tmp, '\'') + 1;
	return (tmp);
}

char	*interpret_double_quotes(	const char **command_input, \
									const char **environement, \
									t_token_type *type)
{
	char		**variable_pointers;
	char		*buffer;
	uint32_t	variable_count;

	variable_count = determine_variables(*command_input);
	variable_pointers = ft_calloc(variable_count + 1, sizeof(char *));
	buffer = NULL;
	if (!ft_strchr(*command_input + 1, '\"') || !variable_pointers)
		lst_memory(NULL, NULL, CLEAN);
	*ft_strchr(*command_input + 1, '\"') = 0;
	if (variable_count)
	{
		extract_variable(variable_pointers, *command_input, \
						environement, variable_count);
		extract_word(&buffer, (char *)*command_input + 1, variable_pointers);
		*type = TOKEN_ENV;
	}
	else
	{
		*type = TOKEN_WORD;
		buffer = (char *)*command_input + 1;
	}
	*command_input = ft_strchr(*command_input + 1, 0);
	return ((*command_input)++, free(variable_pointers), buffer);
}
