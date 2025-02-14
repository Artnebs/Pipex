#include "pipex.h"

char **parse_command(char *cmd_str)
{
	char **cmd_args;

	cmd_args = ft_split(cmd_str, ' ');
	if (cmd_args == NULL)
	{
		perror("Error: Command parsing failed");
		exit(EXIT_FAILURE);
	}
	return (cmd_args);
}

char *find_command_path(char *cmd_name)
{
	char **paths;
	char *full_path;
	int idx;

	paths = ft_split(getenv("PATH"), ':');
	if (paths == NULL)
		return (NULL);
	idx = 0;
	while (paths[idx])
	{
		full_path = ft_strjoin(paths[idx], "/");
		full_path = ft_strjoin_free(full_path, cmd_name);
		if (access(full_path, X_OK) == 0)
		{
			ft_free_split(paths);
			return (full_path);
		}
		free(full_path);
		idx = idx + 1;
	}
	ft_free_split(paths);
	return (NULL);
}
