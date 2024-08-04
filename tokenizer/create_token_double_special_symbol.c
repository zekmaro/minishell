/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_token_double_special_symbol.c               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 12:35:12 by anarama           #+#    #+#             */
/*   Updated: 2024/08/02 17:43:40 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_double_special(const char *input)
{
	return ((ft_strncmp(input, ">>", 2) == 0)
		|| (ft_strncmp(input, "<<", 2) == 0)
		|| (ft_strncmp(input, "&&", 2) == 0)
		|| (ft_strncmp(input, "||", 2) == 0));
}

char	*token_heredoc_input_get(t_prompt *heredoc, const char *delimiter)
{
	char	*heredoc_input;

	heredoc_input = prompt_get_input(heredoc, \
									PROMPT_INPUT_BUFFER_SIZE, \
									delimiter);
	if (!heredoc_input || !*heredoc_input)
		return (NULL);
	return (heredoc_input);
}

void	token_heredoc_get(t_token *token, char **input)
{
	static t_prompt	heredoc;
	char			*temp_move;

	heredoc = (t_prompt){0};
	temp_move = *input;
	while (*input && ft_isspace(**input))
		(*input)++;
	if (*input && ft_isalnum(**input) \
		&& !is_double_special(*input) \
		&& !is_single_special(**input))
	{
		while (*temp_move && (ft_isprint(*temp_move) \
				&& !ft_isspace(*temp_move) \
				&& !is_double_special(temp_move) \
				&& !is_single_special(*temp_move)))
			temp_move++;
		*temp_move = 0;
	}
	heredoc.prompt_length = prompt_display_string_set(&heredoc, \
													NULL, \
													"heredoc> ");
	token_heredoc_input_get(&heredoc, *input);
	*input = temp_move + 1;
}

t_token	create_token_double_special_symbol(char **input)
{
	t_token			temp_token;
	t_token_type	token_type;

	if (ft_strncmp(*input, ">>", 2) == 0)
		token_type = TOKEN_REDIRECT_APPEND;
	else if (ft_strncmp(*input, "<<", 2) == 0)
		token_heredoc_get(&temp_token, input);
	else if (ft_strncmp(*input, "&&", 2) == 0)
		token_type = TOKEN_AND;
	else
		token_type = TOKEN_OR;
	temp_token = create_token(token_type, NULL);
	**input = 0;
	*input += 2;
	return (temp_token);
}
