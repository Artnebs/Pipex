/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 15:20:04 by anebbou           #+#    #+#             */
/*   Updated: 2025/02/26 18:35:23 by anebbou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	close_pipes(int pipe_fd[2])
{
	close(pipe_fd[0]);
	close(pipe_fd[1]);
}

void	create_pipe(int pipe_fd[2])
{
	if (pipe(pipe_fd) < 0)
	{
		perror("Pipe creation failed");
		exit(EXIT_FAILURE);
	}
}

void	create_fork(pid_t *child_pid)
{
	*child_pid = fork();
	if (*child_pid < 0)
	{
		perror("Fork failed");
		exit(EXIT_FAILURE);
	}
}
