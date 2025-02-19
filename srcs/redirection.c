/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 15:20:35 by anebbou           #+#    #+#             */
/*   Updated: 2025/02/19 15:58:17 by anebbou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	redirect_input(char *infile)
{
	int	fd_in;

	fd_in = open(infile, O_RDONLY);
	if (fd_in < 0)
	{
		perror("Error opening input file");
		fd_in = open("/dev/null", O_RDONLY);
		if (fd_in < 0)
		{
			perror("Error opening /dev/null");
			exit(EXIT_FAILURE);
		}
	}
	if (dup2(fd_in, STDIN_FILENO) < 0)
	{
		perror("Error redirecting input");
		close(fd_in);
		exit(EXIT_FAILURE);
	}
	close(fd_in);
}

void	redirect_output(char *outfile)
{
	int	fd_out;

	fd_out = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
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

void	redirect_output_append(char *outfile)
{
	int	fd_out;

	fd_out = open(outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd_out < 0)
	{
		perror("Error opening output file in append mode");
		exit(EXIT_FAILURE);
	}
	if (dup2(fd_out, STDOUT_FILENO) < 0)
	{
		perror("Error redirecting output in append mode");
		close(fd_out);
		exit(EXIT_FAILURE);
	}
	close(fd_out);
}
