#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE
#include "minishell.h"
#include "libft/libft.h"

void	rl_print_error_and_reset_prompt(char *msg)
{
	ft_putendl_fd(msg, 2);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	print_heredoc_warning(char *delim, t_data *data)
{
	char	*msg;
	char	*temp;
	char	*temp2;
	char	*line_str;

	line_str = ft_itoa(data->current_line);
	temp = ft_strjoin("minishell: warning: here-document at line ", line_str);
	temp2 = ft_strjoin(" delimited by end-of-file (wanted '", delim);
	msg = ft_strjoin3(temp, temp2, "')");
	ft_putendl_fd(msg, 2);
	free(line_str);
	free(temp);
	free(temp2);
	free(msg);
}
