#include "pipex.h"

void close_pipes(int pipe_fd[2])
{
	close(pipe_fd[0]);
	close(pipe_fd[1]);
}

void create_pipe(int pipe_fd[2])
{
	int ret;

	ret = pipe(pipe_fd);
	if (ret < 0)
	{
		perror("Pipe creation failed");
		exit(EXIT_FAILURE);
	}
}

void create_fork(pid_t *child_pid)
{
	*child_pid = fork();
	if (*child_pid < 0)
	{
		perror("Fork failed");
		exit(EXIT_FAILURE);
	}
}
