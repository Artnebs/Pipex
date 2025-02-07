/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 15:23:41 by anebbou           #+#    #+#             */
/*   Updated: 2025/02/07 16:19:16 by anebbou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	validate_files(char *file1, char *file2)
{
	if (access(file1, F_OK) != 0)
	{
		perror("Error: Input file does not exist");
		exit(EXIT_FAILURE);
	}
	if (access(file2, F_OK) == 0 && access(file2, W_OK) != 0)
	{
		perror("Error: Output file is not writable");
		exit(EXIT_FAILURE);
	}
}

char	**parse_command(char *cmd)
{
	char	**cmd_args;

	cmd_args = ft_split(cmd, ' ');
	if (!cmd_args)
	{
		perror("Error: Command parsing failed");
		exit(EXIT_FAILURE);
	}
	return (cmd_args);
}

char	*find_command_path(char *cmd, char **envp)
{
	char	**paths;
	char	*cmd_path;
	int		i;

	paths = ft_split(getenv("PATH"), ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		cmd_path = ft_strjoin(paths[i], "/");
		cmd_path = ft_strjoin(cmd_path, cmd);
		if (access(cmd_path, X_OK) == 0)
		{
			ft_free_split(paths);
			return (cmd_path);
		}
		free(cmd_path);
		i++;
	}
	ft_free_split(paths);
	return (NULL);
}
