/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiple_pipes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 18:10:20 by anebbou           #+#    #+#             */
/*   Updated: 2025/01/18 15:19:36 by anebbou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
** execute_multiple_pipes - Executes multiple commands in a pipeline.
** @file1:  Input file path (optional; can be NULL in here_doc mode).
** @file2:  Output file path (required).
** @cmds:   Array of command strings.
** @cmd_count: Number of commands in cmds.
** @envp:   Environment variables array.
** @here_doc:  If 1 => last command's output should be appended to file2;
**            if 0 => last command's output should truncate file2.
**
** This function sets up dynamic pipes for all commands, hooking:
**   - the first command to either file1 or STDIN (if here_doc was set up),
**   - intermediate commands to read from the previous pipe,
**   - the last command to write to file2 (append or truncate).
*/
void execute_multiple_pipes(char *file1, char *file2,
							char **cmds, int cmd_count,
							char **envp, int here_doc)
{
	int i, status;
	int pipes[cmd_count - 1][2];
	pid_t pids[cmd_count];

	/* Create pipes for each "gap" between commands */
	for (i = 0; i < cmd_count - 1; i++)
	{
		if (pipe(pipes[i]) < 0)
		{
			perror("Pipe creation failed");
			exit(EXIT_FAILURE);
		}
	}

	for (i = 0; i < cmd_count; i++)
	{
		pids[i] = fork();
		if (pids[i] < 0)
		{
			perror("Fork failed");
			exit(EXIT_FAILURE);
		}
		if (pids[i] == 0)
		{
			/* Child process */
			if (i == 0)
			{
				/* first command */
				if (file1 != NULL)
				{
					int fd_in = open(file1, O_RDONLY);
					if (fd_in < 0)
					{
						perror("Error opening input file");
						exit(EXIT_FAILURE);
					}
					dup2(fd_in, STDIN_FILENO);
					close(fd_in);
				}
				/* else if file1 == NULL => reading from STDIN (heredoc) */
			}
			else
			{
				/* Not the first command => read from previous pipe */
				dup2(pipes[i - 1][0], STDIN_FILENO);
			}

			if (i == cmd_count - 1)
			{
				/* last command => write to file2 */
				int fd_out;
				if (here_doc)
					fd_out = open(file2, O_WRONLY | O_CREAT | O_APPEND, 0644);
				else
					fd_out = open(file2, O_WRONLY | O_CREAT | O_TRUNC, 0644);
				if (fd_out < 0)
				{
					perror("Error opening output file");
					exit(EXIT_FAILURE);
				}
				dup2(fd_out, STDOUT_FILENO);
				close(fd_out);
			}
			else
			{
				/* not last => write to next pipe */
				dup2(pipes[i][1], STDOUT_FILENO);
			}

			/* Close all pipes in child */
			for (int j = 0; j < cmd_count - 1; j++)
			{
				close(pipes[j][0]);
				close(pipes[j][1]);
			}

			/* Parse and execute the command */
			char **cmd_args = parse_command(cmds[i]);
			execute_command(cmd_args, envp);
			/* If exec fails, we exit. But we do a safety exit here: */
			ft_free_split(cmd_args);
			exit(EXIT_FAILURE);
		}
	}

	/* Parent closes all pipes */
	for (i = 0; i < cmd_count - 1; i++)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
	}

	/* Wait for all children */
	for (i = 0; i < cmd_count; i++)
		waitpid(pids[i], &status, 0);
}
