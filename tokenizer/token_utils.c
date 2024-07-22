/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 19:43:16 by anarama           #+#    #+#             */
/*   Updated: 2024/07/22 14:00:31 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_token(void *addr_token)
{
	t_token	*temp;

	temp = (t_token *)addr_token;
	if (temp)
	{
		if (temp->token_value)
		{
			free(temp->token_value);
			temp->token_value = NULL;
		}
		free(temp);
	}
}

void	free_tokens_arr(void *addr_tokens)
{
	t_token	*temp;
	int		i;

	temp = (t_token *)addr_tokens;
	i = 0;
	while (temp[i].token_type != TOKEN_EOL)
	{
		if (temp[i].token_type == TOKEN_ENV)
			free(temp[i].token_value);
		i++;
	}
	if (temp)
		free(temp);
}

void	print_tokens(t_token *tokens)
{
	int	i;

	i = 0;
	while (tokens[i].token_type != TOKEN_EOL)
	{
		printf("Token: Type=%d, Value=%s\n",
			tokens[i].token_type, tokens[i].token_value);
		i++;
	}
}

void	copy_token_info(t_token **dest, t_token *src)
{
	char	*temp;

	**dest = create_token(src->token_type, src->token_value);
	if (!*dest)
	{
		perror("Token creation failed");
		lst_memory(NULL, NULL, CLEAN);
	}
}

t_token	**custom_realloc(void **tokens, int old_capacity,
			int new_capacity, int add_to_lst)
{
	int		i;
	t_token	**new_tokens;

	new_tokens = ft_calloc(new_capacity + 1, sizeof(void *));
	if (!new_tokens)
	{
		perror("malloc failed while reallocing memory");
		lst_memory(NULL, NULL, CLEAN);
	}
	if (add_to_lst)
		lst_memory(new_tokens, free_tokens_arr, ADD);
	i = 0;
	while (i < old_capacity)
	{
		copy_token_info(&new_tokens[i], tokens[i]);
		i++;
	}
	return (new_tokens);
}
