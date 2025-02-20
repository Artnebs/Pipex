/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 15:19:25 by anebbou           #+#    #+#             */
/*   Updated: 2025/02/20 16:41:06 by anebbou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	read_stdin_loop(int write_fd, char *limiter)
{
	char	*line;

	line = get_next_line(STDIN_FILENO);
	while (line != NULL)
	{
		if ((ft_strncmp(line, limiter, ft_strlen(limiter)) == 0)
			&& (line[ft_strlen(limiter)] == '\n'))
		{
			free(line);
			break ;
		}
		write(write_fd, line, ft_strlen(line));
		free(line);
		line = get_next_line(STDIN_FILENO);
	}
}

void	child_write_here_doc(int *pipefd, t_here_doc doc)
{
	close(pipefd[0]);
	read_stdin_loop(pipefd[1], doc.limiter);
	close(pipefd[1]);
	exit(EXIT_SUCCESS);
}

void	handle_here_doc(t_here_doc doc)
{
	int		pipefd[2];
	pid_t	child_pid;
	int		fd_out;

	create_pipe_or_exit(pipefd);
	fork_or_exit(&child_pid);
	if (child_pid == 0)
		child_write_here_doc(pipefd, doc);
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	fd_out = open(doc.outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_out < 0)
	{
		perror("Error opening output file");
		exit(EXIT_FAILURE);
	}
	dup2(fd_out, STDOUT_FILENO);
	close(fd_out);
	execute_command(parse_command(doc.cmds[0]), doc.envp);
	waitpid(child_pid, NULL, 0);
}
