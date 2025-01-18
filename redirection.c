/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 17:03:55 by anebbou           #+#    #+#             */
/*   Updated: 2025/01/17 18:27:19 by anebbou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void setup_redirection(char *file1, char *file2)
{
	int fd_in = open(file1, O_RDONLY);
	if (fd_in < 0)
		perror("Error opening input file"), exit(EXIT_FAILURE);

	if (dup2(fd_in, STDIN_FILENO) < 0)
		perror("Error redirecting input"), exit(EXIT_FAILURE);

	close(fd_in);

	int fd_out = open(file2, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_out < 0)
		perror("Error opening output file"), exit(EXIT_FAILURE);

	if (dup2(fd_out, STDOUT_FILENO) < 0)
		perror("Error redirecting output"), exit(EXIT_FAILURE);

	close(fd_out);
}
