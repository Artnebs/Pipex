/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 17:00:00 by anebbou           #+#    #+#             */
/*   Updated: 2025/02/21 11:56:48 by anebbou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	create_pipe_or_exit(int *pipefd)
{
	if (pipe(pipefd) == -1)
	{
		perror("Pipe creation failed");
		exit(EXIT_FAILURE);
	}
}

void	fork_or_exit(pid_t *child_pid)
{
	*child_pid = fork();
	if (*child_pid == -1)
	{
		perror("Fork failed");
		exit(EXIT_FAILURE);
	}
}

void	redirect_output_or_exit(char *outfile)
{
	int	fd_out;

	fd_out = open(outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
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

void	create_here_doc_tempfile(int _fd, char *temp_filename)
{
	int		fd_temp;
	char	buffer[1024];
	ssize_t	bytes;

	fd_temp = open(temp_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_temp < 0)
	{
		perror("Error opening temporary file");
		exit(EXIT_FAILURE);
	}
	bytes = read(_fd, buffer, sizeof(buffer));
	while (bytes > 0)
	{
		if (write(fd_temp, buffer, bytes) != bytes)
		{
			perror("Error writing to temporary file");
			close(fd_temp);
			exit(EXIT_FAILURE);
		}
		bytes = read(_fd, buffer, sizeof(buffer));
	}
	close(fd_temp);
}
