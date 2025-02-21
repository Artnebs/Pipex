/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_helpers_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 15:35:15 by anebbou           #+#    #+#             */
/*   Updated: 2025/02/21 11:55:37 by anebbou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	print_usage_bonus(void)
{
	ft_putstr_fd("Usage (Bonus):\n", 2);
	ft_putstr_fd("  ./pipex here_doc LIMITER cmd1 cmd2 ... cmdn outfile\n", 2);
	ft_putstr_fd(" multiple pipes: ./pipex infile cmd1 ... cmdn outfile\n", 2);
}

static int	run_here_doc_mode(int argc, char **argv, char **envp)
{
	t_here_doc	doc_info;

	doc_info.limiter = argv[2];
	doc_info.cmds = &argv[3];
	doc_info.cmd_count = argc - 4;
	doc_info.outfile = argv[argc - 1];
	doc_info.envp = envp;
	handle_here_doc(doc_info);
	return (EXIT_SUCCESS);
}

static int	run_multi_pipe_mode(int argc, char **argv, char **envp)
{
	t_multi_pipes	multi_info;

	multi_info.infile = argv[1];
	multi_info.cmds = &argv[2];
	multi_info.cmd_count = argc - 3;
	multi_info.outfile = argv[argc - 1];
	multi_info.envp = envp;
	multi_info.here_doc = 0;
	execute_multiple_pipes(multi_info);
	return (EXIT_SUCCESS);
}

int	handle_args_bonus(int argc, char **argv, char **envp)
{
	if (argc > 5 && ft_strncmp(argv[1], "here_doc", 8) == 0)
		return (run_here_doc_mode(argc, argv, envp));
	else if (argc == 5)
	{
		redirect_input(argv[1]);
		redirect_output(argv[4]);
		execute_pipeline(parse_command(argv[2]), parse_command(argv[3]), envp);
		return (EXIT_SUCCESS);
	}
	else if (argc > 5)
		return (run_multi_pipe_mode(argc, argv, envp));
	print_usage_bonus();
	return (EXIT_FAILURE);
}
