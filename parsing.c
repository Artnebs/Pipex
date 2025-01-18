/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 15:23:41 by anebbou           #+#    #+#             */
/*   Updated: 2025/01/18 15:20:17 by anebbou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/**
 * validate_files - Validates that input and output files can be accessed.
 * @file1: Input file path.
 * @file2: Output file path.
 *
 * Checks if the input file can be opened for reading and the output file
 * can be created or opened for writing. Exits the program on failure.
 */
void validate_files(char *file1, char *file2)
{
	int fd = open(file1, O_RDONLY);
	if (fd < 0)
	{
		perror("Error opening input file");
		exit(EXIT_FAILURE);
	}
	close(fd);

	fd = open(file2, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("Error opening or creating output file");
		exit(EXIT_FAILURE);
	}
	close(fd);
}

/**
 * parse_command - Splits a command string into an array of arguments.
 * @cmd: Command string (e.g., "ls -l /usr/bin").
 *
 * Returns:
 *   A dynamically allocated array of strings containing the command and
 *   its arguments. Exits the program on failure.
 */
char **parse_command(char *cmd)
{
	char **split_cmd = ft_split(cmd, ' ');
	if (!split_cmd || !split_cmd[0])
	{
		ft_printf("Error: Invalid or empty command: '%s'\n", cmd);
		if (split_cmd)
			ft_free_split(split_cmd);
		exit(EXIT_FAILURE);
	}
	return (split_cmd);
}

/**
 * find_command_path - Finds the full path of a command.
 * @cmd: Command name.
 * @envp: Array of environment variables.
 *
 * Returns:
 *   A dynamically allocated string containing the full path of the command.
 *   Returns NULL if the command is not found.
 */
char *find_command_path(char *cmd, char **envp)
{
	char **paths;
	char *path_var;
	int i;

	/* If cmd has a slash, user might have given an absolute/relative path. */
	if (ft_strchr(cmd, '/')) // [CHANGED] use ft_strchr
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd)); // [CHANGED] use ft_strdup
		return (NULL);
	}

	/* Locate PATH in envp */
	path_var = NULL;
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path_var = envp[i] + 5; // skip "PATH="
			break;
		}
		i++;
	}
	if (!path_var)
		return (NULL);

	/* Split PATH into directories */
	paths = ft_split(path_var, ':');
	if (!paths)
		return (NULL);

	/* Check each directory for the command */
	i = 0;
	while (paths[i])
	{
		char *tmp;
		char *full;

		/* build "dir + '/'" */
		tmp = ft_strjoin(paths[i], "/"); // [CHANGED] ft_strjoin
		if (!tmp)
		{
			ft_free_split(paths);
			return (NULL);
		}
		/* build the full path "tmp + cmd" */
		full = ft_strjoin(tmp, cmd); // [CHANGED] ft_strjoin
		free(tmp);
		if (!full)
		{
			ft_free_split(paths);
			return (NULL);
		}
		if (access(full, X_OK) == 0)
		{
			ft_free_split(paths);
			return (full);
		}
		free(full);
		i++;
	}
	ft_free_split(paths);
	return (NULL);
}
