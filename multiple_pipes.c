/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiple_pipes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 18:10:20 by anebbou           #+#    #+#             */
/*   Updated: 2025/01/18 14:10:00 by anebbou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/**
 * execute_multiple_pipes - Executes multiple commands connected via pipes.
 * @file1: Input file path (not used in this implementation).
 * @file2: Output file path (not used in this implementation).
 * @cmds: Array of command strings.
 * @cmd_count: Number of commands.
 * @envp: Array of environment variables.
 *
 * Sets up dynamic pipes for the commands and uses fork to execute each
 * command in a separate child process. Cleans up resources after execution.
 */
void execute_multiple_pipes(char *file1, char *file2, char **cmds, int cmd_count, char **envp)
{
	(void)file1;
	(void)file2;

	int pipes[cmd_count - 1][2];
	pid_t pids[cmd_count];

	for (int i = 0; i < cmd_count - 1; i++)
		if (pipe(pipes[i]) < 0)
		{
			perror("Pipe creation failed");
			exit(EXIT_FAILURE);
		}

	for (int i = 0; i < cmd_count; i++)
	{
		pids[i] = fork();
		if (pids[i] < 0)
		{
			perror("Fork failed");
			exit(EXIT_FAILURE);
		}

		if (pids[i] == 0) // Child process
		{
			if (i > 0)
				dup2(pipes[i - 1][0], STDIN_FILENO);
			if (i < cmd_count - 1)
				dup2(pipes[i][1], STDOUT_FILENO);

			for (int j = 0; j < cmd_count - 1; j++)
			{
				close(pipes[j][0]);
				close(pipes[j][1]);
			}

			char **cmd_args = parse_command(cmds[i]);
			execute_command(cmd_args, envp);
			ft_free_split(cmd_args); // Free allocated memory
		}
	}

	for (int i = 0; i < cmd_count - 1; i++)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
	}

	for (int i = 0; i < cmd_count; i++)
		waitpid(pids[i], NULL, 0);
}