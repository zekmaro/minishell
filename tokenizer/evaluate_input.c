/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   evaluate_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 14:40:58 by vvobis            #+#    #+#             */
/*   Updated: 2024/08/04 14:21:15 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

uint32_t	evaluate_variable(	char **input, \
								uint32_t input_length, \
								const char **environment, \
								int32_t *exit_status)
{
	char		*input_new;
	uint32_t	i;
	char		*temp_move;
	char		character_store;
	char		*string_to_insert;
	uint32_t	string_to_insert_length;

	string_to_insert = NULL;
	input_new = ft_calloc(input_length * 2, sizeof(*input_new));
	if (!input_new)
	{
		perror("ft_calloc");
		lst_memory(NULL, NULL, CLEAN);
	}
	ft_strlcpy(input_new, *input, input_length + 1);
	i = 0;
	while (input_new[i])
	{
		if (input_new[i] == '\'')
		{
			temp_move = ft_strchr(&input_new[i + 1], '\'');
			if (!temp_move)
			{
				p_stderr(2, "%s: Invalid SINGLE Quotes\n", *input);
				lst_memory(NULL, NULL, CLEAN);
			}
			else
			{
				i = temp_move - input_new + 1;
			}
		}
		else if (input_new[i] == '$')
		{
			ft_memmove(&input_new[i], &input_new[i + 1], input_length - i);
			if (input_new[i] == '?' && (ft_isspace(input_new[i + 1]) || input_new[i + 1] == 0))
				string_to_insert = ft_itoa(*exit_status);
			temp_move = &input_new[i];
			while (temp_move[0] && temp_move[0] != ' ' \
					&& temp_move[0] != '\'' \
					&& temp_move[0] != '\"' \
					&& temp_move[0] != '$')
				temp_move++;
			character_store = *temp_move;
			*temp_move = 0;
			if (!string_to_insert)
			{
				string_to_insert = environment_variable_value_get(&input_new[i], \
						environment);
				string_to_insert_length = ft_strlen(string_to_insert);
				*temp_move = character_store;
				input_length = ft_strlen(input_new);
				ft_memmove(&input_new[i + string_to_insert_length], \
						&input_new[temp_move - input_new], \
						input_length - (temp_move - input_new) + 1);
				ft_memcpy(&input_new[i], string_to_insert, string_to_insert_length);
			}
			else
			{
				string_to_insert_length = ft_strlen(string_to_insert);
				*temp_move = character_store;
				input_length = ft_strlen(input_new);
				ft_memmove(&input_new[i + string_to_insert_length], \
						&input_new[temp_move - input_new], \
						input_length - (temp_move - input_new) + 1);
				ft_memcpy(&input_new[i], string_to_insert, string_to_insert_length);
				ft_free(&string_to_insert);
			}
			i += string_to_insert_length - 1;
		}
		i++;
	}
	*input = input_new;
	return (ft_strlen(input_new));
}

void	evaluate_double_quotes(char **input, uint32_t *i, uint32_t input_length)
{
	char		*temp_move;
	uint32_t	original_i;

	original_i = *i;
	ft_memmove(&(*input)[*i], &(*input)[*i + 1], input_length - *i);
	input_length--;
	temp_move = ft_strchr(*input, '\"');
	if (!temp_move)
	{
		UNIMPLEMENTED("Error single quote missing");
		lst_memory(NULL, NULL, CLEAN);
	}
	while ((*input)[*i])
	{
		if ((*input)[*i] == '\"')
		{
			ft_memmove(&(*input)[*i], &(*input)[*i + 1], input_length - *i);
			input_length--;
			break ;
		}
		(*i)++;
	}
	*i = original_i;
}

void	evaluate_single_quotes(char **input, uint32_t *i, uint32_t input_length)
{
	char	*temp_move;

	ft_memmove(&(*input)[*i], &(*input)[*i + 1], input_length - *i);
	input_length--;
	temp_move = ft_strchr(&(*input)[*i], '\'');
	if (!temp_move)
	{
		UNIMPLEMENTED("Error single quote missing");
		lst_memory(NULL, NULL, CLEAN);
	}
	while ((*input)[*i] != '\'')
		(*i)++;
	temp_move = &(*input)[*i];
	ft_memmove(temp_move, temp_move + 1, ft_strlen(temp_move));
}

char	**evaluate_input(char ***input, const char **environment, int32_t *exit_status)
{
	uint32_t	input_length;
	uint32_t	i;
	uint32_t	j;

	i = 0;
	while ((*input)[i])
	{
		input_length = ft_strlen((*input)[i]);
		j = 0;
		if (ft_strchr((*input)[i], '$'))
		{
			input_length = evaluate_variable(&(*input)[i], \
											input_length, \
											environment, exit_status);
		}
		while ((*input)[i][j])
		{
			if ((*input)[i][j] == '\"')
				evaluate_double_quotes(&(*input)[i], &j, input_length);
			else if ((*input)[i][j] == '\'')
				evaluate_single_quotes(&(*input)[i], &j, input_length);
			j++;
		}
		i++;
	}
	return (*input);
}
