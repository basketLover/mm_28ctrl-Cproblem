/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdolores <mdolores@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 13:35:12 by mariserr          #+#    #+#             */
/*   Updated: 2025/09/30 11:26:48 by mdolores         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE 
#include "minishell.h" 
#include "libft/libft.h" 

static int	write_heredoc_line(int fd, char *line, int quoted,
		t_data *data)
{
	char	*expanded;

	if (!quoted)
	{
		expanded = expand_token(line, data);
		if (!expanded)
			return (free(line), 1);
		ft_putendl_fd(expanded, fd);
		free(expanded);
	}
	else
		ft_putendl_fd(line, fd);
	free(line);
	return (0);
}

int	write_heredoc_input(int fd, char *delim, int delim_quoted,
		t_data *data)
{
	char	*line;

	if (!delim)
		return (1);
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			print_heredoc_warning(delim, data);
			break ;
		}
		if (ft_strcmp(line, delim) == 0)
		{
			free(line);
			line = NULL;
			break ;
		}
		if (write_heredoc_line(fd, line, delim_quoted, data) != 0)
			return (1);
	}
	rl_clear_history();
	return (0);
}

static void	handle_heredoc_child(int *pipefd, char *delim,
		int delim_quoted, t_data *data)
{
	struct sigaction	sa_hd;

	sa_hd.sa_handler = heredoc_sigint;
	sigemptyset(&sa_hd.sa_mask);
	sa_hd.sa_flags = 0;
	sigaction(SIGINT, &sa_hd, NULL);
	signal(SIGQUIT, SIG_IGN);
	close(pipefd[0]);
	data->heredoc_open_line = get_current_line(data);
	write_heredoc_input(pipefd[1], delim, delim_quoted, data);
	close(pipefd[1]);
	_exit(0);
}

static int	handle_heredoc_parent(pid_t pid, int *pipefd,
		t_data *data)
{
	struct sigaction	old_int;
	struct sigaction	ign;
	int					status;

	ign.sa_handler = SIG_IGN;
	sigemptyset(&ign.sa_mask);
	ign.sa_flags = 0;
	sigaction(SIGINT, &ign, &old_int);
	waitpid(pid, &status, 0);
	sigaction(SIGINT, &old_int, NULL);
	if (WEXITSTATUS(status) == 130)
	{
		close(pipefd[0]);
		data->heredoc_interrupted = 1;
		return (-1);
	}
	return (pipefd[0]);
}

int	handle_heredoc(char *delim, int delim_quoted, t_data *data)
{
	int		pipefd[2];
	pid_t	pid;

	if (!delim)
		return (-1);
	if (pipe(pipefd) < 0)
		return (perror("pipe"), 1);
	pid = fork();
	if (pid < 0)
		return (perror("fork"), close(pipefd[0]), close(pipefd[1]), -1);
	if (pid == 0)
		handle_heredoc_child(pipefd, delim, delim_quoted, data);
	close(pipefd[1]);
	return (handle_heredoc_parent(pid, pipefd, data));
}
