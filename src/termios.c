/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termios.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 09:46:56 by vvobis            #+#    #+#             */
/*   Updated: 2024/07/28 00:16:11 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	terminal_raw_mode_enable(int flag)
{
	struct termios	raw;

	tcgetattr(STDIN_FILENO, &raw);
    raw.c_lflag &= ~(flag);
	tcsetattr(STDIN_FILENO, TCSANOW, &raw);
}

void	terminal_raw_mode_disable(int flag)
{
	struct termios	orig;

	tcgetattr(STDIN_FILENO, &orig);
    orig.c_lflag |= (flag);
	tcsetattr(STDIN_FILENO, TCSANOW, &orig);
}
