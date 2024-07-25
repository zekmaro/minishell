#include "minishell.h"

int main()
{
	char c;

	terminal_raw_mode_enable();
	while (1)
	{
		read(1, &c, 1);
		printf("%d\n", c);
	}
	terminal_raw_mode_disable();
}
