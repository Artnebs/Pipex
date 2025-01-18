/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiple_pipes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 18:10:20 by anebbou           #+#    #+#             */
/*   Updated: 2025/01/17 18:40:34 by anebbou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void execute_multiple_pipes(char *file1, char *file2, char **cmds, int cmd_count, char **envp)
{
	(void)file1;
	(void)file2;
	
	int pipes[cmd_count - 1][2];
	pid_t pids[cmd_count];

	for (int i = 0; i < cmd_count - 1; i++)
		if (pipe(pipes[i]) < 0)
			perror("Pipe creation failed"), exit(EXIT_FAILURE);

	for (int i = 0; i < cmd_count; i++)
	{
		pids[i] = fork();
		if (pids[i] < 0)
			perror("Fork failed"), exit(EXIT_FAILURE);
		if (pids[i] == 0)
		{
			if (i > 0)
				dup2(pipes[i - 1][0], STDIN_FILENO);
			if (i < cmd_count - 1)
				dup2(pipes[i][1], STDOUT_FILENO);

			for (int j = 0; j < cmd_count - 1; j++)
				close(pipes[j][0]), close(pipes[j][1]);

			execute_command(parse_command(cmds[i]), envp);
		}
	}

	for (int i = 0; i < cmd_count - 1; i++)
		close(pipes[i][0]), close(pipes[i][1]);

	for (int i = 0; i < cmd_count; i++)
		waitpid(pids[i], NULL, 0);
}
