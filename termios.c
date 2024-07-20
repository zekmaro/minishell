/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termios.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 09:46:56 by vvobis            #+#    #+#             */
/*   Updated: 2024/07/20 13:22:54 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	terminal_raw_mode_enable(void)
{
	struct termios	raw;

	tcgetattr(STDIN_FILENO, &raw);
	raw.c_lflag &= ~(ICANON | ECHO | ECHOCTL);
	tcsetattr(STDIN_FILENO, TCSANOW, &raw);
	ft_printf(SCREEN_ENABLE_WRAPPING);
}

void	terminal_raw_mode_disable(void)
{
	struct termios	orig;

	tcgetattr(STDIN_FILENO, &orig);
	orig.c_lflag |= (ICANON | ECHO | ECHOCTL);
	tcsetattr(STDIN_FILENO, TCSANOW, &orig);
	ft_printf(SCREEN_DISBLE_WRAPPING);
}
