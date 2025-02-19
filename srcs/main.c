/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 15:20:25 by anebbou           #+#    #+#             */
/*   Updated: 2025/02/19 15:53:38 by anebbou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	validate_environment(char **envp)
{
	int	i;

	i = 0;
	if (envp == NULL || envp[0] == NULL)
	{
		ft_putstr_fd("Error: Missing environment variables.\n", 2);
		exit(EXIT_FAILURE);
	}
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return ;
		i = i + 1;
	}
	ft_putstr_fd("Error: PATH variable is missing in the environment.\n", 2);
	exit(EXIT_FAILURE);
}

int	main(int argc, char **argv, char **envp)
{
	int	ret;

	validate_environment(envp);
	ret = handle_args(argc, argv, envp);
	return (ret);
}
