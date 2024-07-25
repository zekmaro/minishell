#include "minishell.h"

int main()
{
	DIR *dir = opendir("builtin");

	while (1)
	{
		struct dirent *entry = readdir(dir);
		if (!entry)
			break ;
		ft_putendl_fd(entry->d_name, 1);
	}
}
