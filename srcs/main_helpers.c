/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 15:35:15 by anebbou           #+#    #+#             */
/*   Updated: 2025/02/20 19:32:49 by anebbou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	print_usage(void)
{
	ft_putstr_fd("Usage (Mandatory):\n", 2);
	ft_putstr_fd("  ./pipex infile cmd1 cmd2 outfile\n", 2);
}

static int	run_standard_mode(char **argv, char **envp)
{
	redirect_input(argv[1]);
	redirect_output(argv[4]);
	execute_pipeline(parse_command(argv[2]), parse_command(argv[3]), envp);
	return (EXIT_SUCCESS);
}

int	handle_args(int argc, char **argv, char **envp)
{
	if (argc == 5)
		return (run_standard_mode(argv, envp));
	print_usage();
	return (EXIT_FAILURE);
}
