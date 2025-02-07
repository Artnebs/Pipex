/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiple_pipes_helpers_bonus.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 18:10:20 by anebbou           #+#    #+#             */
/*   Updated: 2025/02/07 17:03:08 by anebbou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	fork_and_execute(int i, int cmd_count, int pipes[][2], pid_t *pids,
							char **cmds, char **envp)
{
	create_fork(&pids[i]);
	if (pids[i] == 0)
		execute_child(i, cmd_count, pipes, cmds, envp);
}

void	fork_processes(int cmd_count, int pipes[][2], pid_t *pids, char **cmds,
						char **envp)
{
	int	i;

	i = 0;
	while (i < cmd_count)
	{
		fork_and_execute(i, cmd_count, pipes, pids, cmds, envp);
		i++;
	}
}