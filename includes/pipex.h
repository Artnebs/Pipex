#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <sys/wait.h>
# include "libft.h"

// Mandatory Structures
typedef struct s_multi_pipes
{
    int		cmd_count;
    char	**envp;
    char    *infile;
    char    *outfile;
    char    **cmds;
}	t_multi_pipes;

typedef struct s_here_doc
{
    char	*limiter;
    char	*outfile;
    char	**cmds;
    int		cmd_count;
    char	**envp;
}	t_here_doc;

/*
** Mandatory
*/
void	validate_environment(char **envp);
void	execute_pipeline(char **command1, char **command2, char **envp);
void	execute_command(char **command_args, char **envp);

char	**parse_command(char *cmd_str);
char	*find_command_path(char *cmd_name);

/*
** Helpers
*/
void	close_pipes(int pipe_fd[2]);
void	create_pipe(int pipe_fd[2]);
void	create_fork(pid_t *child_pid);
void	redirect_input(char *infile);
void	redirect_output(char *outfile);

/*
** Bonus
*/
void	handle_here_doc(t_here_doc doc_info);
void	execute_multiple_pipes(t_multi_pipes multi);

/*
** Bonus helpers
*/
void	close_all_pipes(int **pipes_array, int count);
int		**allocate_pipes(int count);
void	free_pipes(int **pipes_array, int count);
void	wait_for_children(pid_t child1, pid_t child2);
void	wait_for_all_children(pid_t *pids, int count);
void	create_pipes(int pipes[][2], int count);
void    fork_processes(t_multi_pipes multi, int pipes[][2], pid_t *pids);

#endif