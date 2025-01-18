/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 21:27:12 by anebbou           #+#    #+#             */
/*   Updated: 2025/01/18 14:34:06 by anebbou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
#define PIPEX_H

/* ************************************************************************** */
/* INCLUDES */
/* ************************************************************************** */

#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include "libft.h"

/* ************************************************************************** */
/* STRUCTURES */
/* ************************************************************************** */

/* ************************************************************************** */
/* PROTOTYPES */
/* ************************************************************************** */

/* main.c */
void validate_environment(char **envp);

/* parsing.c */
void validate_files(char *file1, char *file2);
char **parse_command(char *cmd);
char *find_command_path(char *cmd, char **envp);

/* redirection.c */
void setup_redirection(char *file1, char *file2);

/* execute.c */
void execute_pipeline(char **cmd1, char **cmd2, char **envp);
void execute_command(char **cmd, char **envp);

/* multiple_pipes.c */
void execute_multiple_pipes(char *file1, char *file2,
							char **cmds, int cmd_count, char **envp, int here_doc);

/* here_doc.c */
void handle_here_doc(char *limiter, char **cmds, int cmd_count,
					 char *file, char **envp);

/* ************************************************************************** */
/* DEBUG */
/* ************************************************************************** */

#endif