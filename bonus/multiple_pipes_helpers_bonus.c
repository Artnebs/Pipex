/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiple_pipes_helpers_bonus.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 15:19:47 by anebbou           #+#    #+#             */
/*   Updated: 2025/02/20 17:14:25 by anebbou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	execute_child_cmd(int idx, t_multi_pipes multi, int **pipes)
{
	if (idx == 0)
	{
		if (multi.infile != NULL)
			redirect_input(multi.infile);
	}
	else
		dup2(pipes[idx - 1][0], STDIN_FILENO);
	if (idx == multi.cmd_count - 1)
	{
		if (multi.here_doc)
			redirect_output_append(multi.outfile);
		else
			redirect_output(multi.outfile);
	}
	else
		dup2(pipes[idx][1], STDOUT_FILENO);
	close_all_pipes(pipes, multi.cmd_count - 1);
	execute_command(parse_command(multi.cmds[idx]), multi.envp);
}

void	fork_processes(t_multi_pipes multi, int **pipes, pid_t *pids)
{
	int	i;

	i = 0;
	while (i < multi.cmd_count)
	{
		create_fork(&pids[i]);
		if (pids[i] == 0)
			execute_child_cmd(i, multi, pipes);
		i = i + 1;
	}
}

void	wait_for_all_children(pid_t *pids, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		waitpid(pids[i], NULL, 0);
		i = i + 1;
	}
}

void	create_pipes_helper(int pipes[][2], int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		create_pipe(pipes[i]);
		i = i + 1;
	}
}
