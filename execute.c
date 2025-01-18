/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 15:23:54 by anebbou           #+#    #+#             */
/*   Updated: 2025/01/18 12:17:21 by anebbou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void execute_pipeline(char *file1, char *file2, char **cmd1, char **cmd2, char **envp)
{
	(void)file1;
	(void)file2;
	
	int pipe_fd[2];
	pid_t pid1, pid2;

	if (pipe(pipe_fd) < 0)
		perror("Pipe creation failed"), exit(EXIT_FAILURE);

	pid1 = fork();
	if (pid1 < 0)
		perror("Fork failed"), exit(EXIT_FAILURE);
	if (pid1 == 0)
	{
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		execute_command(cmd1, envp);
	}

	pid2 = fork();
	if (pid2 < 0)
		perror("Fork failed"), exit(EXIT_FAILURE);
	if (pid2 == 0)
	{
		dup2(pipe_fd[0], STDIN_FILENO);
		close(pipe_fd[1]);
		close(pipe_fd[0]);
		execute_command(cmd2, envp);
	}

	close(pipe_fd[0]);
	close(pipe_fd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
}

void execute_command(char **cmd, char **envp)
{
	if (execve(cmd[0], cmd, envp) < 0)
	{
		perror("Execution failed");
		ft_free_split(cmd);
		exit(EXIT_FAILURE);
	}
}

void execute_pipe(char **cmd1, char **cmd2, char **envp)
{
	execute_pipeline(NULL, NULL, cmd1, cmd2, envp);
}
