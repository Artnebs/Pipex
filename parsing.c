/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 15:23:41 by anebbou           #+#    #+#             */
/*   Updated: 2025/02/07 12:06:13 by anebbou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void validate_files(char *file1, char *file2)
{
    int fd;

    fd = open(file1, O_RDONLY);
    if (fd < 0)
    {
        perror("Error opening input file");
        exit(EXIT_FAILURE);
    }
    close(fd);
    fd = open(file2, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
    {
        perror("Error opening or creating output file");
        exit(EXIT_FAILURE);
    }
    close(fd);
}

char **parse_command(char *cmd)
{
    char **split_cmd;

    split_cmd = ft_split(cmd, ' ');
    if (!split_cmd || !split_cmd[0])
    {
        ft_printf("Error: Invalid command: %s\n", cmd);
        if (split_cmd)
            ft_free_split(split_cmd);
        exit(EXIT_FAILURE);
    }
    return (split_cmd);
}

static char *check_absolute_path(char *cmd)
{
    if (ft_strchr(cmd, '/'))
    {
        if (access(cmd, X_OK) == 0)
            return (ft_strdup(cmd));
        return (NULL);
    }
    return (NULL);
}

static char *search_in_paths(char *cmd, char **paths)
{
    char *full_path;
    char *temp;
    int i;

    i = 0;
    while (paths[i])
    {
        full_path = ft_strjoin(paths[i], "/");
        temp = full_path;
        full_path = ft_strjoin(full_path, cmd);
        free(temp);
        if (access(full_path, X_OK) == 0)
        {
            ft_free_split(paths);
            return (full_path);
        }
        free(full_path);
        i++;
    }
    return (NULL);
}

char *find_command_path(char *cmd, char **envp)
{
    char **paths;
    char *path_var = NULL;
    char *cmd_path;
    int i = 0;

    cmd_path = check_absolute_path(cmd);
    if (cmd_path)
        return (cmd_path);
    while (envp[i])
    {
        if (ft_strncmp(envp[i], "PATH=", 5) == 0)
        {
            path_var = envp[i] + 5;
            break;
        }
        i++;
    }
    if (!path_var)
        return (NULL);
    paths = ft_split(path_var, ':');
    if (!paths)
        return (NULL);
    return (search_in_paths(cmd, paths));
}
