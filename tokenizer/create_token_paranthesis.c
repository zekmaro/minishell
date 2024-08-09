#include "../minishell.h"
#include <stdbool.h>

t_token	create_token_paranthesis(char	*input, const char **environment, bool	*syntax_error)
{
	t_token token;
	char	*temp_move;

	temp_move = ft_strrchr(input, ')');
	if (!temp_move)
		return (ft_putstr_fd("minishell: closing bracket expected\n", 2), *syntax_error = true, token);
	if (*input == '$' && *input + 1 == '(')
	{
		token.token_type = TOKEN_WORD;
		token.token_value = execute_subshell(input, environment);
	}
	else if (*input == '(')
	{
	}
	return (token);
}