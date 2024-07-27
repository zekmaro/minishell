/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tab_completion.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 14:53:58 by victor            #+#    #+#             */
/*   Updated: 2024/07/28 01:47:44 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	handle_tab_no_match(const char *input_path, const char **env)
{
	uint32_t		cursor_position_stored[2];

	cursor_position_save();
	ft_putstr_fd("\n\r", 1);
	ft_putstr_fd(SCREEN_CLEAR_TO_EOF, 0);
	command_execute("/usr/bin/ls", (const char *[]){"ls", input_path, NULL}, env);
	cursor_position_restore();
}

static char	*determine_word(char *input, char **input_path, uint32_t cursor_position_current)
{
	char			*current_word;
	char			*current_word_path_end;
	char			*tmp;
	uint32_t		i;

	if (!input || !*input)
		return (NULL);
	i = cursor_position_current - (cursor_position_current > 0);
	current_word_path_end = NULL;
	while (i > 0 && input[i] != ' ')
		i--;
	if (i)
		current_word = &input[i + 1];
	else
		current_word = &input[i];
	tmp = ft_strchr(current_word, '/');
	while (tmp)
	{
		current_word_path_end = tmp;
		tmp = ft_strchr(tmp + 1, '/');
	}
	if (current_word_path_end)
	{
		*input_path = ft_substr(current_word, 0, current_word_path_end - current_word + 1);
		current_word = current_word_path_end + 1;
		return (current_word);
	}
	return (current_word);
}

uint32_t	find_last_matching_char(const char *current_word, const char *next_word_match)
{
	uint32_t	i;
	char		*current_word_without_path;

	if (!next_word_match)
		return (0);
	current_word_without_path = ft_strrchr(current_word, '/');
	i = 0;
	if (current_word_without_path)
		while ((current_word_without_path)[i] == next_word_match[i])
			i++;
	else
		while (current_word[i] == next_word_match[i])
			i++;
	return (i);
}

static char *find_next_match(char *current_word, uint32_t current_word_length, DIR *directory_current)
{
	struct	dirent	*directory_entry;
	char			*next_word_match;

	next_word_match = NULL;
	if (!current_word || !directory_current || !*current_word)
		return (NULL);
	while (1)
	{
		directory_entry = readdir(directory_current);
		if (directory_entry == NULL)
			break ;
		if (ft_strncmp(current_word, directory_entry->d_name, current_word_length) == 0)
			next_word_match = directory_entry->d_name;
	}
	return (next_word_match);
}

uint32_t	get_current_word_length(char *word)
{
	char	*word_end;

	if (!word)
		return (0);
	else if (*word == ' ')
		return (1);
	word_end = ft_strchr(word, ' ');
	if (word_end)
		return (word_end - word);
	return (ft_strlen(word));
}

void	handle_tab(char **input, const char **env, uint32_t *cursor_position_current)
{
	DIR				*directory_current;
	char			*current_word;
	char			*next_word_match;
	char			*input_path;
	uint32_t		current_word_length;

	input_path = NULL;
	current_word = determine_word(*input, &input_path, *cursor_position_current);
	current_word_length = get_current_word_length(current_word);
	if (input_path)
		ft_opendir(&directory_current, input_path);
	else
		ft_opendir(&directory_current, "./");
	next_word_match = find_next_match(current_word, current_word_length, directory_current);
	if (!next_word_match)
		handle_tab_no_match(input_path, env);
	else
	{
		handle_multiple_character_to_input(input, next_word_match + current_word_length, cursor_position_current, ft_strlen(*input));
		cursor_position_save();
		ft_putstr_fd("\n\r", 1);
		ft_putstr_fd(SCREEN_CLEAR_TO_EOF, 0);
		cursor_position_restore();
	}
	closedir(directory_current);
}
