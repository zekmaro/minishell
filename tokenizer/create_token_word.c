/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_token_word.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 12:40:09 by anarama           #+#    #+#             */
/*   Updated: 2024/08/04 10:44:08 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*determine_path(char *input)
{
	uint32_t	i;

	i = ft_strlen(input);
	while (i > 0)
	{
		if (input[i] == '/')
			return (input[i] = 0, input);
		else if (input[i] == ' ')
			return (NULL);
		i--;
	}
	return (NULL);
}

uint32_t	get_matches_size(const char *path, const char *to_match)
{
	uint32_t		size;
	uint32_t		to_match_length;
	DIR				*tmp_dir;
	struct dirent	*tmp_entry;

	size = 0;
	ft_opendir(&tmp_dir, path);
	to_match_length = ft_strlen(to_match);
	while (1)
	{
		tmp_entry = readdir(tmp_dir);
		if (!tmp_entry)
			break ;
		if (ft_strncmp(to_match, tmp_entry->d_name, to_match_length) == 0)
			size += ft_strlen(tmp_entry->d_name) + 1;
	}
	closedir(tmp_dir);
	return (size);
}

void	copy_matches(	const char *path, \
						const char *to_match, \
						char **directory_entries)
{
	uint32_t		to_match_length;
	DIR				*tmp_dir;
	struct dirent	*tmp_entry;

	if (*to_match == '*')
		return (copy_matches(path, "0", directory_entries));
	*directory_entries = ft_calloc(get_matches_size(path, to_match) + 1, \
									sizeof(**directory_entries));
	if (!directory_entries)
		lst_memory(NULL, NULL, CLEAN);
	ft_opendir(&tmp_dir, path);
	to_match_length = ft_strlen(to_match);
	while (1)
	{
		tmp_entry = readdir(tmp_dir);
		if (!tmp_entry)
			break ;
		if (ft_strncmp(to_match, tmp_entry->d_name, to_match_length) == 0)
		{
			ft_strlcpy(*directory_entries + ft_strlen(*directory_entries), \
						tmp_entry->d_name, ft_strlen(tmp_entry->d_name) + 1);
			ft_strlcpy(*directory_entries + \
						ft_strlen(*directory_entries), " ", 2);
		}
	}
	closedir(tmp_dir);
}

t_token	expand_wildcard(const char **input)
{
	t_token	temp_token;
	char	*input_path;
	char	*tmp;
	char	*directory_entries;

	tmp = (char *)*input;
	input_path = determine_path((char *)*input);
	if (input_path)
	{
		*input = ft_strrchr(*input, 0) + 1;
		input_path = tmp;
		copy_matches(input_path, *input, &directory_entries);
	}
	else
	{
		*ft_strrchr(*input, '*') = 0;
		copy_matches("./", *input, &directory_entries);
	}
	temp_token.token_type = TOKEN_ENV;
	temp_token.token_value = directory_entries;
	return (temp_token);
}

t_token	create_token_word(const char **input)
{
	t_token	temp_token;
	char	*temp_move;
	char	*input_next;

	temp_move = (char *)*input;
	input_next = NULL;
	temp_token = create_token(TOKEN_WORD, *input);
	while (*temp_move && !ft_isspace(*temp_move))
	{
		if (*temp_move == '\'' || *temp_move == '\"')
			while (*temp_move && (*temp_move != '\'' || *temp_move == '\"'))
				temp_move++;
		temp_move++;
	}
	if (*temp_move)
		input_next = temp_move + 1;
	*temp_move = 0;
	temp_move = ft_strchr(*input, '*');
	if (input_next)
		*input = input_next;
	else
		*input = ft_strchr(*input, 0);
	return (temp_token);
}
