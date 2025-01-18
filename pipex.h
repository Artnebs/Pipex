/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 21:27:12 by anebbou           #+#    #+#             */
/*   Updated: 2025/01/18 14:12:24 by anebbou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

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

void validate_files(char *file1, char *file2);
char **parse_command(char *cmd);
void execute_pipeline(char **cmd1, char **cmd2, char **envp);
void setup_redirection(char *file1, char *file2);
void execute_command(char **cmd, char **envp);
char *find_command_path(char *cmd, char **envp);


/* ************************************************************************** */
/* BONUS */
/* ************************************************************************** */

void execute_multiple_pipes(char *file1, char *file2, char **cmds, int cmd_count, char **envp);
void handle_here_doc(char *limiter, char **cmds, int cmd_count, char *file, char **envp);

/* ************************************************************************** */
/* DEBUG */
/* ************************************************************************** */


#endif