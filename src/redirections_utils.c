/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariserr <mariserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 13:31:22 by mariserr          #+#    #+#             */
/*   Updated: 2025/09/27 13:31:24 by mariserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE
#include "minishell.h"
#include "libft/libft.h"

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
				if (r->fd >= 0)
					close(r->fd);
				g_heredoc_interrupted = 0;
				r->fd = handle_heredoc(r->target, r->quoted, data);
				if (r->fd < 0)
				{
					if (g_heredoc_interrupted)
					{
						/* Heredoc was interrupted by Ctrl+C, just return without error message */
						return (-1);
					}
					else
					{
						ft_putendl_fd("minishell: failed to create heredoc", 2);
						return (-1);
					}
				}
			}
			r = r->next;
		}
		cur = cur->next;
	}
	return (0);
}
