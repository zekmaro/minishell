/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 19:23:40 by anarama           #+#    #+#             */
/*   Updated: 2024/07/30 23:57:31 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	prompt_destroy(void *prompt)
{
	t_prompt	*prompt_ptr;

	prompt_ptr = (t_prompt *)prompt;
	ft_free(&prompt_ptr->history_entries);
	ft_free(&prompt_ptr);
}

uint32_t	prompt_display(const char **environment)
{
	char		*pwd;
	uint32_t	prompt_length;

	pwd = environment_variable_value_get("PWD", environment);
	prompt_length = ft_strlen(pwd) + 4;
	ft_putstr_fd(GREEN, 1);
	ft_printf("[%s]$ ", pwd);
	ft_putstr_fd(RESET, 1);
	return (prompt_length);
}

char	*prompt_get(const char **environment)
{
	char			*input;
	static t_prompt prompt;

	if (!prompt.exists)
		prompt = prompt_create(environment);
	prompt.prompt_length = prompt_display((const char **)prompt.env_ptr);
	prompt.command = prompt_get_input(&prompt, PROMPT_INPUT_BUFFER_SIZE, "\n");
	if (!prompt.command || !*prompt.command)
		return (NULL);
	prompt.history_entries[prompt.history_count] = prompt.command;
	prompt.history_count++;
	prompt.history_position_current = prompt.history_count;
	input = ft_strdup(prompt.command);
	if (!input)
		return (perror("malloc"), lst_memory(NULL, NULL, CLEAN), NULL);
	return (input);
}

t_prompt	prompt_create(const char **env)
{
	t_prompt	tmp;

	tmp = (t_prompt){0};
	tmp.history_entries = ft_calloc(16, sizeof(*tmp.history_entries));
	lst_memory(tmp.history_entries, free, ADD);
	tmp.history_count = 0;
	tmp.env_ptr = (char **)env;
	tmp.exists = true;
	return (tmp);
}
