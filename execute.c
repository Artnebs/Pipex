/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 15:23:54 by anebbou           #+#    #+#             */
/*   Updated: 2025/02/07 12:18:05 by anebbou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	execute_first_command(char **cmd1, int pipe_fd[2], char **envp)
{
	dup2(pipe_fd[1], STDOUT_FILENO);
	close_pipes(pipe_fd);
	execute_command(cmd1, envp);
}

static void	execute_second_command(char **cmd2, int pipe_fd[2], char **envp)
{
	dup2(pipe_fd[0], STDIN_FILENO);
	close_pipes(pipe_fd);
	execute_command(cmd2, envp);
}

void	execute_pipeline(char **cmd1, char **cmd2, char **envp)
{
	int		pipe_fd[2];
	pid_t	pid1;
	pid_t	pid2;

	create_pipe(pipe_fd);
	create_fork(&pid1);
	if (pid1 == 0)
		execute_first_command(cmd1, pipe_fd, envp);
	create_fork(&pid2);
	if (pid2 == 0)
		execute_second_command(cmd2, pipe_fd, envp);
	close_pipes(pipe_fd);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
}

void	execute_command(char **cmd, char **envp)
{
	char	*cmd_path;

	cmd_path = find_command_path(cmd[0], envp);
	if (!cmd_path)
	{
		ft_putstr_fd("Execution failed: Command not found: ", 2);
		ft_putstr_fd(cmd[0], 2);
		ft_putstr_fd("\n", 2);
		ft_free_split(cmd);
		exit(EXIT_FAILURE);
	}
	if (execve(cmd_path, cmd, envp) < 0)
	{
		perror("Execution failed");
		ft_free_split(cmd);
		free(cmd_path);
		exit(EXIT_FAILURE);
	}
	free(cmd_path);
}
