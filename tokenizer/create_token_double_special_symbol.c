/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_token_double_special_symbol.c               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 12:35:12 by anarama           #+#    #+#             */
/*   Updated: 2024/07/30 23:04:20 by vvobis           ###   ########.fr       */
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

void	token_heredoc(t_token *token, char **input)
{
	static	t_prompt prompt;

	prompt = (t_prompt){0};
	/*token->token_value = prompt_get_input(&prompt);*/
}

t_token	create_token_double_special_symbol(char **input)
{
	t_token			temp_token;
	t_token_type	token_type;

	if (ft_strncmp(*input, ">>", 2) == 0)
		token_type = TOKEN_REDIRECT_APPEND;
	else if (ft_strncmp(*input, "<<", 2) == 0)
		token_type = TOKEN_HEREDOC;
	else if (ft_strncmp(*input, "&&", 2) == 0)
		token_type = TOKEN_AND;
	else
		token_type = TOKEN_OR;
	temp_token = create_token(token_type, NULL);
	**input = 0;
	*input += 2;
	return (temp_token);
}
