/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdolores <mdolores@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 13:34:55 by mariserr          #+#    #+#             */
/*   Updated: 2025/09/28 20:49:22 by mdolores         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE
#include "minishell.h"
#include "libft/libft.h"

long	get_current_line(t_data *data)
{
	return (data->current_line);
}

int	swap_stdin_with_fd(int fd)
{
	int	saved_in;

	saved_in = dup(STDIN_FILENO);
	if (saved_in < 0)
	{
		perror("dup");
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		perror("dup2");
		close(saved_in);
		return (-1);
	}
	close(fd);
	return (saved_in);
}

/*void    heredoc_sigint(int sig)
{
    (void)sig;
    g_sig = 1;           // tell the heredoc loop to abort
    write(1, "\n", 1);   // move to a clean line
    rl_replace_line("", 0);
    rl_done = 1;         // force readline() to return
}*/
