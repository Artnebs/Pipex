/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiple_pipes_helpers_bonus.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 15:19:47 by anebbou           #+#    #+#             */
/*   Updated: 2025/02/26 18:49:49 by anebbou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	execute_child_cmd_multi(int idx, t_multi_pipes multi, int **pipes,
									char ***parsed_cmds)
{
	if (idx == 0 && multi.infile)
		redirect_input(multi.infile);
	else if (idx > 0)
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
	execute_command(parsed_cmds[idx], multi.envp);
}

void	fork_processes_multi(t_multi_pipes multi, int **pipes, pid_t *pids,
			char ***parsed_cmds)
{
	int	i;

	i = 0;
	while (i < multi.cmd_count)
	{
		create_fork(&pids[i]);
		if (pids[i] == 0)
			execute_child_cmd_multi(i, multi, pipes, parsed_cmds);
		i++;
	}
}

void	wait_for_all_children(pid_t *pids, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		waitpid(pids[i], NULL, 0);
		i++;
	}
}

void	create_pipes_helper(int pipes[][2], int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		create_pipe(pipes[i]);
		i++;
	}
}

void	close_and_free_pipes(int **pipes, int pipe_count)
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
