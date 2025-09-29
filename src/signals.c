/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdolores <mdolores@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 14:58:22 by iumorave          #+#    #+#             */
/*   Updated: 2025/09/28 22:51:44 by mdolores         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE
#include "minishell.h"
#include "libft/libft.h"

int g_sig = 0;
static struct termios	g_termios_orig;

void	disable_echoctl(void)
{
    struct termios t;

    if (tcgetattr(STDIN_FILENO, &g_termios_orig) == -1)
        return ;
    t = g_termios_orig;
    t.c_lflag &= ~ECHOCTL;
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

void	restore_echoctl(void)
{
    if (g_termios_orig.c_cflag)
        tcsetattr(STDIN_FILENO, TCSANOW, &g_termios_orig);
}

void	heredoc_sigint(int sig)
{
    (void)sig;
    write(STDOUT_FILENO, "\n", 1);
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
    rl_crlf();
    rl_redisplay();
}

void	setup_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = handle_sigint;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &sa_int, NULL) == -1)
		perror("sigaction SIGINT");
	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	if (sigaction(SIGQUIT, &sa_quit, NULL) == -1)
		perror("sigaction SIGQUIT");
}

void	setup_child_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}
