/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiple_pipes_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 15:19:34 by anebbou           #+#    #+#             */
/*   Updated: 2025/02/21 11:54:55 by anebbou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static pid_t	*allocate_pids(int count)
{
	pid_t	*pids;

	pids = (pid_t *)malloc(sizeof(pid_t) * count);
	if (pids == NULL)
	{
		perror("Memory allocation failed");
		exit(EXIT_FAILURE);
	}
	return (pids);
}

int	**allocate_pipes(int pipe_count)
{
	int	**pipes;
	int	i;

	pipes = malloc(sizeof(int *) * pipe_count);
	if (!pipes)
	{
		perror("Memory allocation for pipes failed");
		exit(EXIT_FAILURE);
	}
	i = 0;
	while (i < pipe_count)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i])
		{
			perror("Memory allocation for pipe pair failed");
			while (--i >= 0)
				free(pipes[i]);
			free(pipes);
			exit(EXIT_FAILURE);
		}
		i++;
	}
	return (pipes);
}

static void	create_pipes(int **pipes, int pipe_count)
{
	int	i;

	i = 0;
	while (i < pipe_count)
	{
		if (pipe(pipes[i]) == -1)
		{
			perror("Pipe creation failed");
			while (i >= 0)
			{
				free(pipes[i]);
				i--;
			}
			free(pipes);
			exit(EXIT_FAILURE);
		}
		i++;
	}
}

static void	close_and_free_pipes(int **pipes, int pipe_count)
{
	int	i;

	i = 0;
	while (i < pipe_count)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		free(pipes[i]);
		i++;
	}
	free(pipes);
}

void	execute_multiple_pipes(t_multi_pipes multi)
{
	int		**pipes;
	pid_t	*pids;
	int		pipe_count;

	if (multi.cmd_count == 2)
	{
		execute_pipeline(parse_command(multi.cmds[0]),
			parse_command(multi.cmds[1]), multi.envp);
		return ;
	}
	pipe_count = multi.cmd_count - 1;
	pids = allocate_pids(multi.cmd_count);
	pipes = allocate_pipes(pipe_count);
	create_pipes(pipes, pipe_count);
	fork_processes(multi, pipes, pids);
	close_and_free_pipes(pipes, pipe_count);
	wait_for_all_children(pids, multi.cmd_count);
	free(pids);
}
