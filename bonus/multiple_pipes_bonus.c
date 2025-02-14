#include "pipex.h"

static pid_t *allocate_pids(int count)
{
	pid_t *pids;

	pids = (pid_t *)malloc(sizeof(pid_t) * count);
	if (pids == NULL)
	{
		perror("Memory allocation failed");
		exit(EXIT_FAILURE);
	}
	return (pids);
}

void execute_multiple_pipes(t_multi_pipes multi)
{
	int pipes[multi.cmd_count - 1][2];
	pid_t *pids;
	int i;

	pids = allocate_pids(multi.cmd_count);
	i = 0;
	while (i < multi.cmd_count - 1)
	{
		if (pipe(pipes[i]) == -1)
		{
			perror("Pipe creation failed");
			exit(EXIT_FAILURE);
		}
		i = i + 1;
	}
	fork_processes(multi, pipes, pids);
	i = 0;
	while (i < multi.cmd_count - 1)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i = i + 1;
	}
	wait_for_all_children(pids, multi.cmd_count);
	free(pids);
}
