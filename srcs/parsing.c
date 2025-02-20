/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 15:20:32 by anebbou           #+#    #+#             */
/*   Updated: 2025/02/20 16:52:11 by anebbou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	**parse_command(char *cmd_str)
{
	char	**cmd_args;

	if (!cmd_str || ft_strlen(cmd_str) == 0)
	{
		ft_putstr_fd("Error: empty command\n", 2);
		exit(EXIT_FAILURE);
	}
	cmd_args = ft_split(cmd_str, ' ');
	if (cmd_args == NULL)
	{
		perror("Error: Command parsing failed");
		exit(EXIT_FAILURE);
	}
	return (cmd_args);
}

char *find_command_path(char *cmd_name)
{
	char **paths;
	char *full_path;
	int idx;

	if (!cmd_name || !*cmd_name)
		return (NULL);
	paths = ft_split(getenv("PATH"), ':');
	if (!paths)
		return (NULL);
	idx = 0;
	while (paths[idx])
	{
		full_path = ft_strjoin(paths[idx], "/");
		full_path = ft_strjoin_free(full_path, cmd_name);
		if (access(full_path, X_OK) == 0)
		{
			ft_free_split(paths);
			return (full_path);
		}
		free(full_path);
		idx++;
	}
	ft_free_split(paths);
	return (NULL);
}
