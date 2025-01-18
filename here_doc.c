/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 18:12:36 by anebbou           #+#    #+#             */
/*   Updated: 2025/01/18 14:11:26 by anebbou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/**
 * handle_here_doc - Handles the here_doc feature.
 * @limiter: String that marks the end of user input.
 * @cmds: Array of command strings.
 * @cmd_count: Number of commands.
 * @file: Output file to append the result.
 * @envp: Array of environment variables.
 *
 * Reads input from the user until the limiter is encountered, then executes
 * the provided commands, passing the input via pipes.
 */
void handle_here_doc(char *limiter, char **cmds, int cmd_count, char *file, char **envp)
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
		// Child process: Write user input to pipe until LIMITER
		close(pipefd[0]); // Close read-end
		ft_printf("heredoc> ");
		while ((line = get_next_line(STDIN_FILENO)))
		{
			if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0 && line[ft_strlen(limiter)] == '\n')
				break; // Stop on LIMITER
			write(pipefd[1], line, ft_strlen(line));
			ft_printf("heredoc> ");
			free(line);
		}
		free(line);
		close(pipefd[1]);
		exit(EXIT_SUCCESS);
	}

	// Parent process: Close write-end and pass heredoc input to commands
	close(pipefd[1]);
	if (cmd_count == 2)
	{
		// Last command appends output to file
		int fd_out = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd_out < 0)
		{
			perror("Error opening output file");
			exit(EXIT_FAILURE);
		}
		dup2(pipefd[0], STDIN_FILENO);
		dup2(fd_out, STDOUT_FILENO);
		close(fd_out);
		char **cmd_args = parse_command(cmds[1]);
		execute_command(cmd_args, envp);
		ft_free_split(cmd_args); // Free allocated memory
	}
	else
	{
		execute_multiple_pipes(NULL, file, cmds, cmd_count, envp);
	}
	close(pipefd[0]);
	waitpid(pid, NULL, 0);
}
