/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 17:58:45 by anebbou           #+#    #+#             */
/*   Updated: 2025/01/18 15:19:25 by anebbou          ###   ########.fr       */
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
 * Handles both the mandatory and bonus parts of the project. Supports:
 * - Two commands
 * - Multiple commands
 * - here_doc (with limiter)
 *
 * Returns:
 *   EXIT_SUCCESS on success, or exits with failure status on error.
 */
int main(int argc, char **argv, char **envp)
{
	validate_environment(envp);

	if (argc < 5)
	{
		ft_printf("Usage:\n");
		ft_printf("  %s file1 cmd1 cmd2 [cmd3 ...] file2\n", argv[0]);
		ft_printf("Or for here_doc:\n");
		ft_printf("  %s here_doc LIMITER cmd1 [cmd2 ...] file\n", argv[0]);
		return (EXIT_FAILURE);
	}

	/* Bonus: here_doc mode */
	if (ft_strncmp(argv[1], "here_doc", ft_strlen("here_doc")) == 0)
	{
		if (argc < 5)
		{
			ft_printf("Usage for here_doc: %s here_doc LIMITER cmd1 [cmd2 ...] file\n",
					  argv[0]);
			return (EXIT_FAILURE);
		}
		handle_here_doc(argv[2], &argv[3], argc - 4, argv[argc - 1], envp);
	}
	else
	{
		/* Standard or multiple-pipes mode */
		validate_files(argv[1], argv[argc - 1]);

		if (argc == 5)
		{
			/* Exactly two commands */
			setup_redirection(argv[1], argv[argc - 1]);
			char **cmd1 = parse_command(argv[2]);
			char **cmd2 = parse_command(argv[3]);
			execute_pipeline(cmd1, cmd2, envp);
			ft_free_split(cmd1);
			ft_free_split(cmd2);
		}
		else
		{
			/* More than two commands */
			execute_multiple_pipes(argv[1], argv[argc - 1],
								   &argv[2], argc - 3, envp, 0);
		}
	}
	return (EXIT_SUCCESS);
}
