/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiple_pipes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 18:10:20 by anebbou           #+#    #+#             */
/*   Updated: 2025/02/07 12:15:33 by anebbou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void close_all_pipes(int pipes[][2], int count)
{
    int i;

    i = 0;
    while (i < count)
    {
        close(pipes[i][0]);
        close(pipes[i][1]);
        i++;
    }
}

static void redirect_input_output(int i, int c, char *f1, char *f2, int p[][2])
{
    if (i == 0)
        redirect_input(f1);
    else
        dup2(p[i - 1][0], STDIN_FILENO);

    if (i == c - 1)
        redirect_output(f2);
    else
        dup2(p[i][1], STDOUT_FILENO);
}

static void execute_child(int i, int c, char *f1, char *f2, int p[][2], char **cmds, char **envp)
{
    redirect_input_output(i, c, f1, f2, p);
    close_all_pipes(p, c - 1);
    execute_command(parse_command(cmds[i]), envp);
}

static void fork_processes(char *file1, char *file2, int cmd_count,
                           int pipes[][2], pid_t *pids, char **cmds, char **envp)
{
    int i;

    i = 0;
    while (i < cmd_count)
    {
        create_fork(&pids[i]);
        if (pids[i] == 0)
            execute_child(i, cmd_count, file1, file2, pipes, cmds, envp);
        i++;
    }
}

void execute_multiple_pipes(char *file1, char *file2, char **cmds,
                            int cmd_count, char **envp)
{
    int pipes[cmd_count - 1][2];
    pid_t pids[cmd_count];
    int i;

    i = 0;
    while (i < cmd_count - 1)
    {
        create_pipe(pipes[i]);
        i++;
    }

    fork_processes(file1, file2, cmd_count, pipes, pids, cmds, envp);
    close_all_pipes(pipes, cmd_count - 1);
    i = 0;
    while (i < cmd_count)
    {
        waitpid(pids[i], NULL, 0);
        i++;
    }
}
