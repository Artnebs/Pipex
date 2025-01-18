/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 15:23:54 by anebbou           #+#    #+#             */
/*   Updated: 2025/01/18 14:22:15 by anebbou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/**
 * execute_pipeline - Executes two commands connected via a pipe.
 * @cmd1: First command with its arguments.
 * @cmd2: Second command with its arguments.
 * @envp: Array of environment variables.
 *
 * Creates a pipe to connect the output of the first command to the input
 * of the second command. Handles child processes for execution.
 */
void execute_pipeline(char **cmd1, char **cmd2, char **envp)
{
	int pipe_fd[2];
	pid_t pid1, pid2;

	if (pipe(pipe_fd) < 0)
	{
		perror("Pipe creation failed");
		exit(EXIT_FAILURE);
	}

	pid1 = fork();
	if (pid1 < 0)
	{
		perror("Fork failed");
		exit(EXIT_FAILURE);
	}
	if (pid1 == 0)
	{
		// First child: Executes cmd1
		dup2(pipe_fd[1], STDOUT_FILENO); // Redirect stdout to pipe write-end
		close(pipe_fd[0]);				 // Close unused read-end
		close(pipe_fd[1]);				 // Close write-end after duplication
		execute_command(cmd1, envp);
	}

	pid2 = fork();
	if (pid2 < 0)
	{
		perror("Fork failed");
		exit(EXIT_FAILURE);
	}
	if (pid2 == 0)
	{
		// Second child: Executes cmd2
		dup2(pipe_fd[0], STDIN_FILENO); // Redirect stdin to pipe read-end
		close(pipe_fd[1]);				// Close unused write-end
		close(pipe_fd[0]);				// Close read-end after duplication
		execute_command(cmd2, envp);
	}

	// Parent process: Close all pipe ends and wait for children
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
}

/**
 * execute_command - Executes a single command using execve.
 * @cmd: Command with its arguments.
 * @envp: Array of environment variables.
 *
 * Uses execve to replace the current process image with the new program.
 * Exits the program on failure.
 */
void execute_command(char **cmd, char **envp)
{
	char *cmd_path = find_command_path(cmd[0], envp);
	if (!cmd_path)
	{
		ft_printf("Execution failed: Command not found: %s\n", cmd[0]);
		ft_free_split(cmd);
		exit(EXIT_FAILURE);
	}
	if (execve(cmd_path, cmd, envp) < 0)
	{
		perror("Execution failed");
		ft_free_split(cmd);
		free(cmd_path);
		exit(EXIT_FAILURE);
	}
	free(cmd_path);
}