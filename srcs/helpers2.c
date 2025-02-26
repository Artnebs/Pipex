/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 15:20:19 by anebbou           #+#    #+#             */
/*   Updated: 2025/02/26 19:13:05 by anebbou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	close_all_pipes(int **pipes_array, int count)
{
	int	idx;

	idx = 0;
	while (idx < count)
	{
		close(pipes_array[idx][0]);
		close(pipes_array[idx][1]);
		idx++;
	}
}

void	allocate_each_pipe(int **pipes_array, int count)
{
	int	idx;

	idx = 0;
	while (idx < count)
	{
		pipes_array[idx] = malloc(sizeof(int) * 2);
		if (!pipes_array[idx])
			return ;
		create_pipe(pipes_array[idx]);
		idx++;
	}
}

char	*ft_strjoin_free(char *s1, const char *s2)
{
	char	*joined;
	size_t	len1;
	size_t	len2;

	if (!s1 || !s2)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	joined = malloc(len1 + len2 + 1);
	if (!joined)
		return (NULL);
	ft_strlcpy(joined, s1, len1 + 1);
	ft_strlcat(joined, s2, len1 + len2 + 1);
	free(s1);
	return (joined);
}
