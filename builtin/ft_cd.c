/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 14:10:08 by vvobis            #+#    #+#             */
/*   Updated: 2024/07/24 16:51:17 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_cd(const char **environment, const char **args)
{
	int32_t		return_value;
	uint32_t	args_size;
	char		*path_home;

	args_size = get_split_size((const char **)args);
	if (args_size > 2)
		return (ft_putendl_fd("minishell: cd: too many arguments", 1));
	else if (args_size == 1)
	{
		path_home = environment_variable_get("HOME", environment);
		if (chdir(path_home) != 0)
			return (perror("cd"));
	}
	else
	{
		if (chdir(args[1]) != 0)
			perror("cd");
	}
}
