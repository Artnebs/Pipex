#include "pipex.h"

static void read_stdin_loop(int write_fd, char *limiter)
{
	char *line;

	line = NULL;
	while ((line = get_next_line(STDIN_FILENO)) != NULL)
	{
		if ((ft_strncmp(line, limiter, ft_strlen(limiter)) == 0) &&
			(line[ft_strlen(limiter)] == '\n'))
		{
			free(line);
			break;
		}
		write(write_fd, line, ft_strlen(line));
		free(line);
	}
}

static void child_write_here_doc(int *pipefd, t_here_doc doc)
{
	close(pipefd[0]);
	read_stdin_loop(pipefd[1], doc.limiter);
	close(pipefd[1]);
	exit(EXIT_SUCCESS);
}

void handle_here_doc(t_here_doc doc)
{
	int pipefd[2];
	pid_t child_pid;

	if (pipe(pipefd) == -1)
	{
		perror("Pipe creation failed");
		exit(EXIT_FAILURE);
	}
	child_pid = fork();
	if (child_pid == -1)
	{
		perror("Fork failed");
		exit(EXIT_FAILURE);
	}
	if (child_pid == 0)
	{
		child_write_here_doc(pipefd, doc);
	}
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	execute_command(parse_command(doc.cmds[0]), doc.envp);
	waitpid(child_pid, NULL, 0);
}
