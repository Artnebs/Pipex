/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 15:23:41 by anebbou           #+#    #+#             */
/*   Updated: 2025/01/17 18:27:31 by anebbou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void validate_files(char *file1, char *file2)
{
	int fd = open(file1, O_RDONLY);
	if (fd < 0)
		perror("Error opening file1"), exit(EXIT_FAILURE);
	close(fd);

	fd = open(file2, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		perror("Error opening file2"), exit(EXIT_FAILURE);
	close(fd);
}

char **parse_command(char *cmd)
{
	char **split_cmd = ft_split(cmd, ' ');
	if (!split_cmd || !split_cmd[0])
	{
		ft_printf("Error: Invalid command: %s\n", cmd);
		if (split_cmd)
			ft_free_split(split_cmd);
		exit(EXIT_FAILURE);
	}
	return (split_cmd);
}
