/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 18:12:36 by anebbou           #+#    #+#             */
/*   Updated: 2025/01/18 15:19:05 by anebbou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/**
 * handle_here_doc - Handles the here_doc feature.
 * @limiter: String that marks the end of user input.
 * @cmds: Array of command strings (cmd1, cmd2, etc.).
 * @cmd_count: Number of commands in cmds.
 * @file: Output file to append the result.
 * @envp: Array of environment variables.
 *
 * Reads input from the user until the limiter is encountered, then executes
 * the provided commands. The final command writes to 'file' in append mode.
 */
void handle_here_doc(char *limiter, char **cmds, int cmd_count,
					 char *file, char **envp)
{
	int pipefd[2];
	pid_t pid;
	char *line;

	if (pipe(pipefd) < 0)
	{
		perror("Pipe creation failed");
		exit(EXIT_FAILURE);
	}

	pid = fork();
	if (pid < 0)
	{
		perror("Fork failed");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		/* Child process: Write user input to pipe until LIMITER */
		close(pipefd[0]); // Close read-end
		ft_printf("heredoc> ");
		while ((line = get_next_line(STDIN_FILENO)))
		{
			/* If user typed limiter + newline => break */
			if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0 && line[ft_strlen(limiter)] == '\n')
			{
				free(line);
				break;
			}
			write(pipefd[1], line, ft_strlen(line));
			free(line);
			ft_printf("heredoc> ");
		}
		close(pipefd[1]);
		exit(EXIT_SUCCESS);
	}
	/* Parent process: close write-end and feed that into the pipeline */
	close(pipefd[1]);

	if (cmd_count == 1)
	{
		/* single command reading from heredoc */
		pid = fork();
		if (pid < 0)
		{
			perror("Fork failed");
			exit(EXIT_FAILURE);
		}
		if (pid == 0)
		{
			dup2(pipefd[0], STDIN_FILENO);
			close(pipefd[0]);
			/* open file in append mode */
			int fd_out = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd_out < 0)
			{
				perror("Error opening output file");
				exit(EXIT_FAILURE);
			}
			dup2(fd_out, STDOUT_FILENO);
			close(fd_out);

			char **cmd_args = parse_command(cmds[0]);
			execute_command(cmd_args, envp);
		}
		close(pipefd[0]);
		waitpid(pid, NULL, 0);
	}
	else
	{
		/* For multiple commands, we pass pipefd[0] as the input */
		int saved_stdin = dup(STDIN_FILENO);
		if (saved_stdin < 0)
		{
			perror("dup failed");
			exit(EXIT_FAILURE);
		}
		if (dup2(pipefd[0], STDIN_FILENO) < 0)
		{
			perror("dup2 failed");
			close(pipefd[0]);
			exit(EXIT_FAILURE);
		}
		close(pipefd[0]);

		/* The first command will read from STDIN (heredoc pipe).
		 * The last command will append to 'file'. */
		execute_multiple_pipes(NULL, file, cmds, cmd_count, envp, 1);

		/* Restore original stdin */
		if (dup2(saved_stdin, STDIN_FILENO) < 0)
			perror("dup2 restore failed");
		close(saved_stdin);
	}
	/* Wait for the heredoc writer child */
	wait(NULL);
}
