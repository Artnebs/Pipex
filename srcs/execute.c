/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 15:33:44 by anebbou           #+#    #+#             */
/*   Updated: 2025/02/26 18:34:25 by anebbou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	execute_command(char **command_args, char **envp)
{
	char	*cmd_path;

	cmd_path = find_command_path(command_args[0]);
	if (!cmd_path)
	{
		ft_putstr_fd("Error: Command not found: ", 2);
		ft_putstr_fd(command_args[0], 2);
		ft_putstr_fd("\n", 2);
		ft_free_split(command_args);
		exit(EXIT_FAILURE);
	}
	if (execve(cmd_path, command_args, envp) < 0)
	{
		perror("Error executing command");
		ft_free_split(command_args);
		free(cmd_path);
		exit(EXIT_FAILURE);
	}
	free(cmd_path);
}

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
	pid_t	child1;
	pid_t	child2;

	create_pipe(pipe_fd);
	create_fork(&child1);
	if (child1 == 0)
		execute_first_command(cmd1, pipe_fd, envp);
	create_fork(&child2);
	if (child2 == 0)
		execute_second_command(cmd2, pipe_fd, envp);
	close_pipes(pipe_fd);
	wait_for_children(child1, child2);
	ft_free_split(cmd1);
	ft_free_split(cmd2);
}

void	wait_for_children(pid_t child1, pid_t child2)
{
	waitpid(child1, NULL, 0);
	waitpid(child2, NULL, 0);
}
