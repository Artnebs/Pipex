/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiple_pipes_utils_bonus.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 10:00:00 by anebbou           #+#    #+#             */
/*   Updated: 2025/02/26 18:33:36 by anebbou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	***cache_commands(char **cmds, int count)
{
	char	***cache;
	int		i;

	cache = malloc(sizeof(char **) * count);
	if (!cache)
	{
		perror("Memory allocation for parsed commands failed");
		exit(EXIT_FAILURE);
	}
	i = 0;
	while (i < count)
	{
		cache[i] = parse_command(cmds[i]);
		i++;
	}
	return (cache);
}

void	free_cached_commands(char ***cache, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		ft_free_split(cache[i]);
		i++;
	}
	free(cache);
}
