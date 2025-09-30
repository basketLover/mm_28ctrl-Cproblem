/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdolores <mdolores@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 13:31:22 by mariserr          #+#    #+#             */
/*   Updated: 2025/09/30 11:57:32 by mdolores         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE
#include "minishell.h"
#include "libft/libft.h"

static int	process_heredoc_redir(t_redir *r, t_data *data)
{
	if (r->fd >= 0)
		close(r->fd);
	data->heredoc_interrupted = 0;
	r->fd = handle_heredoc(r->target, r->quoted, data);
	if (r->fd < 0)
	{
		if (data->heredoc_interrupted)
			return (-1);
		else
		{
			ft_putendl_fd("minishell: failed to create heredoc", 2);
			return (-1);
		}
	}
	return (0);
}

int	prepare_heredocs_for_cmds(t_cmd *cmds, t_data *data)
{
	t_cmd	*cur;
	t_redir	*r;

	cur = cmds;
	while (cur)
	{
		r = cur->redirs;
		while (r)
		{
			if (r->type == T_HEREDOC)
			{
				if (process_heredoc_redir(r, data) < 0)
					return (-1);
			}
			r = r->next;
		}
		cur = cur->next;
	}
	return (0);
}
