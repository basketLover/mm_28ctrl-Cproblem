/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdolores <mdolores@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 14:58:29 by iumorave          #+#    #+#             */
/*   Updated: 2025/09/30 19:45:26 by mdolores         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE
#include "minishell.h"
#include "libft/libft.h"

static void	remove_env_var(char **env, char *key)
{
	int	i;
	int	count;

	count = 0;
	while (env[count])
		count++;
	i = 0;
	while (i < count)
	{
		if (ft_strncmp(env[i], key, ft_strlen(key)) == 0
			&& (env[i][ft_strlen(key)] == '='
			|| env[i][ft_strlen(key)] == '\0'))
		{
			free(env[i]);
			while (i < count - 1)
			{
				env[i] = env[i + 1];
				i++;
			}
			env[count - 1] = NULL;
			break ;
		}
		i++;
	}
}

int	unset_builtin(char **args, t_data *data)
{
	int		i;
	int		err;
	char	*msg;

	if (!args[1])
		return (0);
	err = 0;
	i = 1;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			msg = ft_strjoin3("unset: '", args[i], "': not a valid \
				identifier");
			ft_putendl_fd(msg, 2);
			free(msg);
			err = 1;
		}
		else
			remove_env_var(data->env, args[i]);
		i++;
	}
	data->last_status = (err == 1);
	return (data->last_status);
}
