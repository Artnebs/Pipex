/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 17:58:45 by anebbou           #+#    #+#             */
/*   Updated: 2025/02/07 16:59:14 by anebbou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	validate_environment(char **envp)
{
	if (!envp || !*envp)
	{
		ft_putstr_fd("Error: Environment not found\n", 2);
		exit(EXIT_FAILURE);
	}
}

int	main(int argc, char **argv, char **envp)
{
	validate_environment(envp);
	if (argc == 5)
		execute_pipeline(parse_command(argv[2]), parse_command(argv[3]), envp);
	else if (argc > 5 && ft_strcmp(argv[1], "here_doc") == 0)
		handle_here_doc(argv[2], &argv[3], argc - 4, argv[argc - 1], envp);
	else if (argc > 5)
		execute_multiple_pipes(argv[1], argv[argc - 1], &argv[2], argc - 3, envp);
	else
	{
		ft_putstr_fd("Usage: ./pipex file1 cmd1 cmd2 file2\n", 2);
		ft_putstr_fd("       ./pipex here_doc LIMITER cmd1 cmd2 ... cmdn file\n", 2);
		exit(EXIT_FAILURE);
	}
	return (0);
}