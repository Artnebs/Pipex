/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiple_pipes_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 18:10:20 by anebbou           #+#    #+#             */
/*   Updated: 2025/02/07 17:02:55 by anebbou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	redirect_input_output(int i, int c, int p[][2])
{
	if (i == 0)
		redirect_input(f1);
	else
		dup2(p[i - 1][0], STDIN_FILENO);
	if (i == c - 1)
		redirect_output(f2);
	else
		dup2(p[i][1], STDOUT_FILENO);
}

static void	execute_child(int i, int c, int p[][2], char **cmds, char **envp)
{
	redirect_input_output(i, c, p);
	close_all_pipes(p, c - 1);
	execute_command(parse_command(cmds[i]), envp);
}

void	execute_multiple_pipes(char *file1, char *file2, char **cmds,
							int cmd_count, char **envp)
{
	int		pipes[cmd_count - 1][2];
	pid_t	pids[cmd_count];
	int		i;

	i = 0;
	while (i < cmd_count - 1)
	{
		create_pipe(pipes[i]);
		i++;
	}
	fork_processes(cmd_count, pipes, pids, cmds, envp);
	close_all_pipes(pipes, cmd_count - 1);
	i = 0;
	while (i < cmd_count)
	{
		waitpid(pids[i], NULL, 0);
		i++;
	}
}
