/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 17:58:45 by anebbou           #+#    #+#             */
/*   Updated: 2025/01/18 14:11:49 by anebbou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/**
 * validate_environment - Ensures the environment variables are valid.
 * @envp: Environment variables array.
 *
 * Checks if the `PATH` variable exists in the environment.
 * Exits the program with an error message if the environment is invalid.
 */
void validate_environment(char **envp)
{
	int i = 0;

	if (!envp || !*envp)
	{
		ft_printf("Error: Missing environment variables.\n");
		exit(EXIT_FAILURE);
	}

	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return; // PATH variable found
		i++;
	}

	ft_printf("Error: PATH variable is missing in the environment.\n");
	exit(EXIT_FAILURE);
}

/**
 * main - Entry point for the pipex program.
 * @argc: Argument count.
 * @argv: Argument vector.
 * @envp: Environment variables.
 *
 * Handles both the mandatory and bonus parts of the project. Supports two
 * commands with a single pipe and extended functionality for here_doc and
 * multiple pipes.
 *
 * Returns:
 *   EXIT_SUCCESS on success, or exits with failure status on error.
 */
int main(int argc, char **argv, char **envp)
{
	// Validate environment variables
	validate_environment(envp);

	if (argc < 5)
	{
		ft_printf("Usage: ./pipex file1 cmd1 cmd2 [cmd3 ...] file2\n");
		return (EXIT_FAILURE);
	}

	if (ft_strncmp(argv[1], "here_doc", ft_strlen("here_doc")) == 0)
	{
		if (argc < 6)
		{
			ft_printf("Usage for here_doc: ./pipex here_doc LIMITER cmd1 ... cmdn file\n");
			return (EXIT_FAILURE);
		}
		handle_here_doc(argv[2], &argv[3], argc - 4, argv[argc - 1], envp);
	}
	else
	{
		validate_files(argv[1], argv[argc - 1]);
		if (argc == 5)
		{
			setup_redirection(argv[1], argv[argc - 1]);
			char **cmd1 = parse_command(argv[2]);
			char **cmd2 = parse_command(argv[3]);
			execute_pipeline(cmd1, cmd2, envp);
			ft_free_split(cmd1);
			ft_free_split(cmd2);
		}
		else
		{
			execute_multiple_pipes(argv[1], argv[argc - 1], &argv[2], argc - 3, envp);
		}
	}
	return (EXIT_SUCCESS);
}