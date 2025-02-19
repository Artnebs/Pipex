/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 15:20:19 by anebbou           #+#    #+#             */
/*   Updated: 2025/02/19 16:43:09 by anebbou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	close_all_pipes(int **pipes_array, int count)
{
	int	index;

	index = 0;
	while (index < count)
	{
		close(pipes_array[index][0]);
		close(pipes_array[index][1]);
		index = index + 1;
	}
}

void	allocate_each_pipe(int **pipes_array, int count)
{
	int	index;

	index = 0;
	while (index < count)
	{
		pipes_array[index] = malloc(sizeof(int) * 2);
		if (pipes_array[index] == NULL)
			return ;
		create_pipe(pipes_array[index]);
		index = index + 1;
	}
}

void	free_pipes(int **pipes_array, int count)
{
	int	index;

	index = 0;
	while (index < count - 1)
	{
		free(pipes_array[index]);
		index = index + 1;
	}
	free(pipes_array);
}

char	*ft_strjoin_free(char *s1, const char *s2)
{
	char	*joined_str;
	size_t	len1;
	size_t	len2;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	joined_str = malloc(len1 + len2 + 1);
	if (joined_str == NULL)
		return (NULL);
	ft_strlcpy(joined_str, s1, len1 + 1);
	ft_strlcat(joined_str, s2, len1 + len2 + 1);
	free(s1);
	return (joined_str);
}
