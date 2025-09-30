/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdolores <mdolores@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 14:58:22 by iumorave          #+#    #+#             */
/*   Updated: 2025/09/30 12:28:23 by mdolores         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE
#include "minishell.h"
#include "libft/libft.h"

void	set_echoctl(t_data *data, int enable)
{
	struct termios	t;

	if (tcgetattr(STDIN_FILENO, &data->termios_orig) == -1)
		return ;
	t = data->termios_orig;
	if (enable)
		t.c_lflag |= ECHOCTL;
	else
		t.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

void	restore_echoctl(t_data *data)
{
	if (data->termios_orig.c_cflag)
		tcsetattr(STDIN_FILENO, TCSANOW, &data->termios_orig);
}

void	heredoc_sigint(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\r\n", 2);
	_exit(130);
}

void	handle_gsig(t_data *data)
{
	if (!g_sig)
		return ;
	if (g_sig == SIGINT)
		data->last_status = 130;
	else
		data->last_status = 128 + g_sig;
	g_sig = 0;
}

void	handle_sigint(int sig)
{
	g_sig = sig;
	rl_replace_line("", 0);
	rl_on_new_line();
	write(STDOUT_FILENO, "\n", 1);
	rl_redisplay();
}
