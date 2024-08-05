/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 21:02:23 by anarama           #+#    #+#             */
/*   Updated: 2024/08/05 22:39:55 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	get_amount_tokens(t_token *tokens)
{
	int i;

	i = 0;
	while (tokens[i].token_type != TOKEN_EOL)
	{
		i++;
	}
	return (i);
}

// Distinguish 4 cases:
// "*\0"
// "*.c\0"
// "ft_*\0"
// "ft_*.c\0"

int	wild_only_suffix(const char *pattern, const char *str,
				char *ptr)
{
	char	*str_suffix;
	size_t	len_str_suffix;
	size_t	len_ptr;

	len_ptr = ft_strlen(ptr);
	str_suffix = ft_strchr(pattern, *(ptr + 1));
	len_str_suffix = ft_strlen(str_suffix);
	if (len_str_suffix != len_ptr)
		return (0);
	return (ft_strncmp(ptr + 1, str_suffix, len_str_suffix));
}

int	wild_only_prefix(const char *pattern, const char *str,
				char *ptr)
{
	char *temp;
	size_t	len_str_prefix;

	temp = ft_strchr(str, *(ptr - 1));
	len_str_prefix = temp - str;
	return (ft_strncmp(pattern, str, len_str_prefix));
}

int match_found(const char *pattern, const char *str)
{
	char	*ptr;
	int result;

	ptr = ft_strchr(pattern, '*');
	if (pattern[0] == '*' && *(ptr + 1) == '\0')
	{
		return (1);
	}
	else if (pattern[0] == '*' && *(ptr + 1) != '\0')
	{
		result = wild_only_suffix(pattern, str, ptr);
	}
	else if (pattern[0] != '*' && *(ptr + 1) == '\0')
	{
		result = wild_only_prefix(pattern, str, ptr);
	}
	else if (pattern[0] != '*' && *(ptr + 1) != '\0')
	{
		result = -1;
		result += wild_only_suffix(pattern, str, ptr);
		result += wild_only_prefix(pattern, str, ptr);
	}
	return (result);
}

// case when not just '*'
char **expand_wildcard(const char *pattern)
{
	struct dirent *entry;
	char **matches;
	int	capacity;
	int	count;
	DIR *dir;

	matches = NULL;
	ft_opendir(&dir, ".");
	entry = readdir(dir);
	count = 0;
	capacity = 10;
	matches = ft_calloc(capacity + 1, sizeof(char *));
	if (!matches)
	{
		perror("calloc wildcards");
		lst_memory(NULL, NULL, CLEAN);
	}
	while (entry != NULL)
	{
		if (match_found(pattern, entry->d_name))
		{
			if (count >= capacity)
			{
				matches = realloc; // rewrite
				capacity *= 2;	
			}
			matches[count] = ft_strdup(entry->d_name);
			count++;
		}
		entry = readdir(dir);
	}
	closedir(dir);
	return (matches);
}

int	check_wildcard(t_token token)
{
	return (ft_strchr(token.token_value, '*') != NULL);
}

void check_and_expand_wildcards(t_token	**tokens)
{
	char **matches;
	int	match_count;
	int	size;
	int i;

	size = get_amount_tokens(*tokens);
	i = 0;
	while ((*tokens)[i].token_type != TOKEN_EOL)
	{
		if (check_wildcard((*tokens)[i]))
		{
			matches = expand_wildcard((*tokens)[i].token_value);
			if (matches)
			{
				match_count = get_tokens_count(matches);
			}
			// add logic to realloc the tokens
		}
		i++;
	}
}