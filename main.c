/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 17:58:45 by anebbou           #+#    #+#             */
/*   Updated: 2025/01/18 20:40:01 by anebbou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void validate_environment(char **envp)
{
	if (!envp || !*envp)
	{
		ft_putstr_fd("Error: Environment variables not found.\n", 2);
		exit(EXIT_FAILURE);
	}
}

void print_usage(char *prog_name)
{
	ft_printf("Usage:\n");
	ft_printf("  %s file1 cmd1 cmd2 [cmd3 ...] file2\n", prog_name);
	ft_printf("Or for here_doc:\n");
	ft_printf("  %s here_doc LIMITER cmd1 [cmd2 ...] file\n", prog_name);
}

void handle_standard_mode(int argc, char **argv, char **envp)
{
	validate_files(argv[1], argv[argc - 1]);
	if (argc == 5)
	{
		setup_redirection(argv[1], argv[argc - 1]);
		{
			char **cmd1 = parse_command(argv[2]);
			char **cmd2 = parse_command(argv[3]);
			execute_pipeline(cmd1, cmd2, envp);
			ft_free_split(cmd1);
			ft_free_split(cmd2);
		}
	}
	else
		execute_multiple_pipes(argv[1], argv[argc - 1],
							   &argv[2], argc - 3, envp);
}

int main(int argc, char **argv, char **envp)
{
	validate_environment(envp);
	if (argc < 5)
	{
		print_usage(argv[0]);
		return (EXIT_FAILURE);
	}
	if (ft_strncmp(argv[1], "here_doc", ft_strlen("here_doc")) == 0)
	{
		if (argc < 5)
		{
			print_usage(argv[0]);
			return (EXIT_FAILURE);
		}
		handle_here_doc(argv[2], &argv[3], argc - 4, argv[argc - 1], envp);
	}
	else
		handle_standard_mode(argc, argv, envp);
	return (EXIT_SUCCESS);
}
