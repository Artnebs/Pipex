/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 15:20:25 by anebbou           #+#    #+#             */
/*   Updated: 2025/02/26 18:41:42 by anebbou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	validate_environment(char **envp)
{
	int	i;

	if (!envp || !envp[0])
	{
		ft_putstr_fd("Error: Missing environment variables.\n", 2);
		exit(EXIT_FAILURE);
	}
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return ;
		i++;
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
