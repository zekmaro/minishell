/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 19:40:20 by vvobis            #+#    #+#             */
/*   Updated: 2024/07/26 20:59:01 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "sys/ioctl.h"

static void	handle_backspace(	char *input, \
								uint32_t *cursor_position_current, \
								uint32_t input_length_current)
{
	if (*cursor_position_current <= 0)
		return ;
	(*cursor_position_current)--;
	ft_memmove(&input[*cursor_position_current], \
				&input[*cursor_position_current + 1], \
				input_length_current - *cursor_position_current);
}

static void	handle_new_character_to_input(	char **input,
											char character,
											uint32_t *cursor_position_current,
											uint32_t prompt_length_current)
{
	if (*cursor_position_current < prompt_length_current)
		ft_memmove(&(*input)[*cursor_position_current + 1], \
					&(*input)[*cursor_position_current], \
					prompt_length_current - *cursor_position_current);
	(*input)[*cursor_position_current] = character;
	(*cursor_position_current)++;
}

static uint8_t	handle_single_char_input(char **input, char character, \
										uint32_t cursor_position_current[2])
{
	uint32_t	input_length_current;

	input_length_current = ft_strlen(*input);
	if (g_signal_flag == 1)
		return (ft_putstr_fd("\n", 1), ft_putstr_fd(SCREEN_CLEAR_TO_EOF, 1), 1);
	else if (ft_isprint(character))
		handle_new_character_to_input(input, character, \
										&cursor_position_current[1], \
										input_length_current);
	else if (character == DEL)
		handle_backspace(*input, &cursor_position_current[1], \
						input_length_current);
	else if (character == '\n')
		return (ft_putstr_fd("\n", 1), ft_putstr_fd(SCREEN_CLEAR_TO_EOF, 1), 1);
	else if (character == 4 && input_length_current == 0)
		return (ft_putstr_fd("\n", 1), lst_memory(NULL, NULL, CLEAN), 0);
	return (0);
}
/**/
/*static void	handle_input(	t_prompt *prompt, \*/
/*							char *input, \*/
/*							uint32_t cursor_position_base)*/
/*{*/
/*	uint32_t	cursor_position[2];*/
/*	char		character;*/
/**/
/*	cursor_position_get(cursor_position);*/
/*	cursor_position[1] = 0;*/
/*	while (1)*/
/*	{*/
/*		ft_read(0, &character, &input, 1);*/
/*		if ((g_signal_flag == 1 || ft_isprint(character) || character == DEL \*/
/*			|| character == '\n' || character == EOT) \*/
/*			&& handle_single_char_input(&input, character, cursor_position))*/
/*			break ;*/
/*		else*/
/*		{*/
/*			if (character == ESC)*/
/*				handle_escape_sequence(prompt, &input, &cursor_position[1]);*/
/*			else if (character == '\t')*/
/*				handle_tab(&input, (const char **)prompt->env_ptr, \*/
/*							&cursor_position[1]);*/
/*		}*/
/*		/*prompt_refresh_line(input, (uint32_t [2]){cursor_position[0], \*/
/*				/*cursor_position[1] + cursor_position_base});*/
/*	}*/
/*	prompt->command = input;*/
/*}*/
void reset_terminal_mode() {
    static struct termios orig_termios;
    tcgetattr(STDIN_FILENO, &orig_termios);
    orig_termios.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
}

void set_terminal_mode() {
    struct termios new_termios;
    tcgetattr(STDIN_FILENO, &new_termios);
    new_termios.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
}

void set_nonblocking_mode(int fd) {
    int flags = 1;  // Set the non-blocking flag
    if (ioctl(fd, FIONBIO, &flags) < 0) {
        perror("ioctl FIONBIO");
        exit(EXIT_FAILURE);
    }
}

static void handle_input(t_prompt *prompt, char *input, uint32_t cursor_position_base) {
    uint32_t cursor_position[2];
    char buffer[PROMPT_INPUT_BUFFER_SIZE];
    ssize_t bytes_read;
    size_t total_bytes = 0;

    set_terminal_mode();
    set_nonblocking_mode(STDIN_FILENO);
    cursor_position_get(cursor_position);
    cursor_position[1] = 0;

    while (1) {
        bytes_read = read(STDIN_FILENO, buffer + total_bytes, PROMPT_INPUT_BUFFER_SIZE - total_bytes);
        if (bytes_read > 0) {
            total_bytes += bytes_read;
            buffer[total_bytes] = '\0';  // Null-terminate the buffer

            for (size_t i = 0; i < total_bytes; ++i) {
                char character = buffer[i];
                if ((isprint(character) || character == DEL || character == '\n' || character == EOT) &&
                    handle_single_char_input(&input, character, cursor_position))
                    break;
                else {
                    if (character == ESC)
                        handle_escape_sequence(prompt, &input, &cursor_position[1]);
                    else if (character == '\t')
                        handle_tab(&input, (const char **)prompt->env_ptr, &cursor_position[1]);
                    cursor_position_restore();
                }
                prompt_refresh_line(input, (uint32_t [2]){cursor_position[0], cursor_position[1] + cursor_position_base});
            }

            // Reset the buffer
            total_bytes = 0;
        }
        // Add some sleep to prevent busy-waiting
        usleep(10000); // Sleep for 10ms
    }
    prompt->command = input;
}
char	*prompt_get_input(t_prompt *prompt)
{
	char		*input;
	uint32_t	cursor_position_base;

	cursor_position_base = prompt->prompt_length + 1;
	input = ft_calloc(PROMPT_INPUT_BUFFER_SIZE, sizeof(*input));
	lst_memory(input, free, ADD);
	cursor_position_save();
	handle_input(prompt, input, cursor_position_base);
	return (input);
}
