/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_memory.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor </var/spool/mail/victor>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 12:21:13 by victor            #+#    #+#             */
/*   Updated: 2024/07/09 22:56:30 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	lst_memory(void *mem, void (*del)(void *c), int mode)
{
	static t_clean	*list;
	t_clean			*new;

	if (mode == CLEAN)
	
		return (terminal_raw_mode_disable(), \
				lst_list_clean(&list), exit(EXIT_FAILURE));
	if (mode == END)
		return (terminal_raw_mode_disable(), lst_list_clean(&list));
	if (mode == FREE)
		return (lst_node_del_clean(&list, mem));
	if (!mem)
		return (lst_list_clean(&list), perror("malloc"), exit(EXIT_FAILURE));
	new = lst_node_new(mem, del);
	if (!new)
		return (del(mem), lst_list_clean(&list), terminal_raw_mode_disable(),
				p_stderr(STDERR_FILENO, "pipex: %s", "lst_node_malloc"), \
				perror(""), exit(EXIT_FAILURE));
	lst_add_back(&list, new);
}

int	p_stderr(int stderr_fd, const char *error, const char *specifier)
{
	int	old_stdout;
	int	count;

	old_stdout = dup(STDOUT_FILENO);
	if (old_stdout == -1)
		return (perror("dup"), -1);
	if (dup2(stderr_fd, STDOUT_FILENO) == -1)
		return (perror("dup2"), close(old_stdout), -1);
	count = ft_printf(error, specifier);
	if (dup2(old_stdout, STDOUT_FILENO) == -1)
		return (perror("dup2"), close(old_stdout), -1);
	close(old_stdout);
	return (count);
}
