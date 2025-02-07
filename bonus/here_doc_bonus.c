/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 15:23:54 by anebbou           #+#    #+#             */
/*   Updated: 2025/02/07 17:02:41 by anebbou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	write_to_pipe(int pipefd[2], char *limiter)
{
	char	*line;

	close(pipefd[0]);
	while ((line = get_next_line(STDIN_FILENO)))
	{
		if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0
			&& line[ft_strlen(limiter)] == '\n')
			break ;
		write(pipefd[1], line, ft_strlen(line));
		free(line);
	}
	free(line);
	close(pipefd[1]);
	exit(EXIT_SUCCESS);
}

static void	handle_last_command(char *file, int pipefd[2], char **cmd,
								char **envp)
{
	int	fd_out;

	fd_out = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd_out < 0)
	{
		perror("Error opening output file");
		exit(EXIT_FAILURE);
	}
	dup2(pipefd[0], STDIN_FILENO);
	dup2(fd_out, STDOUT_FILENO);
	close(fd_out);
	execute_command(cmd, envp);
	ft_free_split(cmd);
}

void	handle_here_doc(char *limiter, char **cmds, int cmd_count, char *file,
					char **envp)
{
	int		pipefd[2];
	pid_t	pid;

	create_pipe(pipefd);
	create_fork(&pid);
	if (pid == 0)
		write_to_pipe(pipefd, limiter);
	close(pipefd[1]);
	if (cmd_count == 2)
		handle_last_command(file, pipefd, parse_command(cmds[1]), envp);
	else
		execute_multiple_pipes(NULL, file, cmds, cmd_count, envp);
	close(pipefd[0]);
	waitpid(pid, NULL, 0);
}
