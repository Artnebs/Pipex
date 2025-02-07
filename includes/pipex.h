/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 21:27:12 by anebbou           #+#    #+#             */
/*   Updated: 2025/02/07 16:57:57 by anebbou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

/* ************************************************************************** */
/* INCLUDES */
/* ************************************************************************** */

# include <stdio.h>
# include <stddef.h>
# include <strings.h>
# include <stdlib.h>
# include <fcntl.h>
# include <unistd.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include "libft.h"

/* ************************************************************************** */
/* STRUCTURES */
/* ************************************************************************** */

/* ************************************************************************** */
/* PROTOTYPES */
/* ************************************************************************** */

/* main.c */
void	validate_environment(char **envp);

/* parsing.c */
void	validate_files(char *file1, char *file2);
char	**parse_command(char *cmd);
char	*find_command_path(char *cmd, char **envp);

/* redirection.c */
void	setup_redirection(char *file1, char *file2);

/* execute.c */
void	execute_pipeline(char **cmd1, char **cmd2, char **envp);
void	execute_command(char **cmd, char **envp);

/* multiple_pipes.c */
void	execute_multiple_pipes(char *file1, char *file2, char **cmds,
			int cmd_count, char **envp);

/* here_doc_bonus.c */
void	handle_here_doc(char *limiter, char **cmds, int cmd_count, char *file,
			char **envp);

/* helpers.c */
void	close_pipes(int pipe_fd[2]);
void	create_pipe(int pipe_fd[2]);
void	create_fork(pid_t *pid);
void	redirect_input(char *file1);
void	redirect_output(char *file2);
void	close_all_pipes(int pipes[][2], int count);

/* multiple_pipes_helpers_bonus.c */
void	fork_and_execute(int i, int cmd_count, char *file1, char *file2,
			int pipes[][2], pid_t *pids, char **cmds, char **envp);
void	fork_processes(char *file1, char *file2, int cmd_count,
			int pipes[][2], pid_t *pids, char **cmds, char **envp);

/* ************************************************************************** */
/* DEBUG */
/* ************************************************************************** */

#endif