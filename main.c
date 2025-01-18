/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 17:58:45 by anebbou           #+#    #+#             */
/*   Updated: 2025/01/18 12:18:17 by anebbou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int main(int argc, char **argv, char **envp)
{
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
			char **cmd1 = parse_command(argv[2]);
			char **cmd2 = parse_command(argv[3]);
			execute_pipe(cmd1, cmd2, envp);
			ft_free_split(cmd1);
			ft_free_split(cmd2);
		}
		else 
			execute_multiple_pipes(argv[1], argv[argc - 1], &argv[2], argc - 3, envp);
	}
	return (EXIT_SUCCESS);
}
