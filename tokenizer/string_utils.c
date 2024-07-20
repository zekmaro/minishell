/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 19:44:57 by anarama           #+#    #+#             */
/*   Updated: 2024/07/20 19:51:07 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int get_len_next_special_char(const char *input) 
{
    int i = 0;
    while (input[i] && !is_special_char(input[i]))
        i++;
    return (i);
}

int get_len_next_space_or_quote(const char *input) 
{
    int i = 0;
    while (input[i] && !ft_isspace(input[i]) && !ft_is_double_quote(input[i], NULL))
        i++;
    return (i);
}

int get_len_next_space(const char *input) 
{
    int i = 0;
    while (input[i] && !ft_isspace(input[i]))
        i++;
    return (i);
}

int get_len_next_single_quote(const char *input)
{
    int i = 0;
    int second_quote_found = 0;

    while (input[i] && !ft_is_single_quote(input[i], &second_quote_found))
        i++;
    if (second_quote_found == 0)
        return (0);
    return (i + 1); // Include the closing quote
}