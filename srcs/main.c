#include "pipex.h"

static void print_usage(void)
{
	ft_putstr_fd("Usage:\n", 2);
	ft_putstr_fd("  ./pipex infile cmd1 cmd2 outfile\n", 2);
#ifdef BONUS
	ft_putstr_fd("  ./pipex here_doc LIMITER cmd1 cmd2 ... cmdn outfile\n", 2);
#endif
}

void validate_environment(char **envp)
{
	int i;

	i = 0;
	if (envp == NULL || envp[0] == NULL)
	{
		ft_putstr_fd("Error: Missing environment variables.\n", 2);
		exit(EXIT_FAILURE);
	}
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return;
		i = i + 1;
	}
	ft_putstr_fd("Error: PATH variable is missing in the environment.\n", 2);
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv, char **envp)
{
	validate_environment(envp);
	if (argc == 5)
	{
		redirect_input(argv[1]);
		redirect_output(argv[4]);
		execute_pipeline(parse_command(argv[2]), parse_command(argv[3]), envp);
	}
#ifdef BONUS
	else if (argc > 5 && ft_strncmp(argv[1], "here_doc", 8) == 0)
	{
		{
			t_here_doc doc_info;

			doc_info.limiter = argv[2];
			doc_info.cmds = &argv[3];
			doc_info.cmd_count = argc - 4;
			doc_info.outfile = argv[argc - 1];
			doc_info.envp = envp;
			handle_here_doc(doc_info);
		}
	}
	else if (argc > 5)
	{
		{
			t_multi_pipes multi_info;

			multi_info.cmd_count = argc - 3;
			multi_info.envp = envp;
			execute_multiple_pipes(multi_info);
		}
	}
#endif
	else
	{
		print_usage();
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
