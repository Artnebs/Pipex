/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 17:03:55 by anebbou           #+#    #+#             */
/*   Updated: 2025/01/18 15:20:24 by anebbou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/**
 * setup_redirection - Sets up file redirection for input and output.
 * @file1: Input file to read from.
 * @file2: Output file to write to (trunc).
 *
 * Opens the input and output files and redirects the standard input
 * and output to these files using dup2. Exits the program on failure.
 * This is used only in the simple 2-command scenario.
 */
void setup_redirection(char *file1, char *file2)
{
	int fd_in = open(file1, O_RDONLY);
	if (fd_in < 0)
	{
		perror("Error opening input file");
		exit(EXIT_FAILURE);
	}

	if (dup2(fd_in, STDIN_FILENO) < 0)
	{
		perror("Error redirecting input");
		close(fd_in);
		exit(EXIT_FAILURE);
	}
	close(fd_in);

	int fd_out = open(file2, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_out < 0)
	{
		perror("Error opening output file");
		exit(EXIT_FAILURE);
	}

	if (dup2(fd_out, STDOUT_FILENO) < 0)
	{
		perror("Error redirecting output");
		close(fd_out);
		exit(EXIT_FAILURE);
	}
	close(fd_out);
}
