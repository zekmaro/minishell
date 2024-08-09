/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termios.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 09:46:56 by vvobis            #+#    #+#             */
/*   Updated: 2024/08/04 13:25:04 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	terminal_raw_mode_enable(int flag)
{
	struct termios	raw;

	if (isatty(0) && isatty(1))
	{
		tcgetattr(STDIN_FILENO, &raw);
		raw.c_lflag &= ~(flag);
		tcsetattr(STDIN_FILENO, TCSANOW, &raw);
	}
}

void	terminal_raw_mode_disable(int flag)
{
	struct termios	orig;

	if (isatty(0) && isatty(1))
	{
		tcgetattr(STDIN_FILENO, &orig);
		orig.c_lflag |= (flag);
		tcsetattr(STDIN_FILENO, TCSANOW, &orig);
	}
}
