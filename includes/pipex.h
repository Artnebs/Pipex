/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 21:27:12 by anebbou           #+#    #+#             */
/*   Updated: 2025/02/20 17:09:38 by anebbou          ###   ########.fr       */
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

typedef struct s_here_doc
{
	char	*limiter;
	char	**cmds;
	int		cmd_count;
	char	*outfile;
	char	**envp;
}	t_here_doc;

typedef struct s_multi_pipes
{
	char	**cmds;
	int		cmd_count;
	char	*infile;
	char	*outfile;
	char	**envp;
	int		here_doc;
}	t_multi_pipes;

/* ************************************************************************** */
/* PROTOTYPES */
/* ************************************************************************** */

/* main.c */
void	validate_environment(char **envp);

/* parsing.c */
char	**parse_command(char *cmd_str);
char	*find_command_path(char *cmd_name);

/* redirection.c */
void	redirect_input(char *infile);
void	redirect_output(char *outfile);
void	redirect_output_append(char *outfile);

/* execute.c */
void	execute_pipeline(char **command1, char **command2, char **envp);
void	execute_command(char **command_args, char **envp);
void	wait_for_children(pid_t child1, pid_t child2);

/* multiple_pipes_bonus.c */
void	execute_multiple_pipes(t_multi_pipes multi);

/* here_doc_bonus.c */
void	handle_here_doc(t_here_doc doc);
void	read_stdin_loop(int write_fd, char *limiter);
void	child_write_here_doc(int *pipefd, t_here_doc doc);

/* here_doc_helpers.c */
void	create_pipe_or_exit(int *pipefd);
void	fork_or_exit(pid_t *child_pid);
void	redirect_output_or_exit(char *outfile);
void	create_here_doc_tempfile(int read_fd, char *temp_filename);

/* helpers.c */
void	close_pipes(int pipe_fd[2]);
void	create_pipe(int pipe_fd[2]);
void	create_fork(pid_t *child_pid);

/* helpers2.c */
void	close_all_pipes(int **pipes_array, int count);
void	allocate_each_pipe(int **pipes_array, int count);
int		**allocate_pipes(int count);
void	free_pipes(int **pipes_array, int count);
char	*ft_strjoin_free(char *s1, const char *s2);

/* multiple_pipes_helpers_bonus.c */
void	fork_processes(t_multi_pipes multi, int **pipes, pid_t *pids);
void	wait_for_all_children(pid_t *pids, int count);
void	create_pipes_helper(int pipes[][2], int count);

/* main_helpers.c */
int		handle_args(int argc, char **argv, char **envp);

/* main_helpers_bonus.c */
int		handle_args_bonus(int argc, char **argv, char **envp);

#endif