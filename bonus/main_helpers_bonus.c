/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_helpers_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 15:19:47 by anebbou           #+#    #+#             */
/*   Updated: 2025/02/26 18:53:25 by anebbou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	handle_here_doc_args(int argc, char **argv, char **envp)
{
	t_here_doc	doc;

	if (argc < 6)
	{
		ft_putstr_fd(
			"Usage: ./pipex_bonus here_doc LIMITER cmd1 cmd2 ... cmdN outfile\n",
			2);
		return (EXIT_FAILURE);
	}
	doc.limiter = argv[2];
	doc.cmds = &argv[3];
	doc.cmd_count = argc - 4;
	doc.outfile = argv[argc - 1];
	doc.envp = envp;
	handle_here_doc(doc);
	return (EXIT_SUCCESS);
}

static int	handle_multi_pipes_args(int argc, char **argv, char **envp)
{
	t_multi_pipes	multi;

	multi.cmds = &argv[2];
	multi.cmd_count = argc - 3;
	multi.infile = argv[1];
	multi.outfile = argv[argc - 1];
	multi.envp = envp;
	multi.here_doc = 0;
	execute_multiple_pipes(multi);
	return (EXIT_SUCCESS);
}

int	handle_args_bonus(int argc, char **argv, char **envp)
{
	if (argc < 5)
	{
		ft_putstr_fd("Usage: ./pipex_bonus infile cmd1... cmdN outfile\n", 2);
		return (EXIT_FAILURE);
	}
	if (!ft_strncmp(argv[1], "here_doc", 8) && argv[1][8] == '\0')
		return (handle_here_doc_args(argc, argv, envp));
	else
		return (handle_multi_pipes_args(argc, argv, envp));
}
