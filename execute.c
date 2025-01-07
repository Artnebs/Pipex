/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 15:23:54 by anebbou           #+#    #+#             */
/*   Updated: 2025/01/07 15:53:44 by anebbou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	execute_pipeline(char *file1, char *file2, char **cmd1, char **cmd2, char **envp)
{
	int pipe_fd[2];
	pid_t	pid1;
	pid_t	pid2;
}