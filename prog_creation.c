/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nroth <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 12:35:06 by nroth             #+#    #+#             */
/*   Updated: 2023/02/14 12:35:08 by nroth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	**ft_arrappend_void(void **arr, void *to_append)
{
	void	**ret;
	int		i;

	i = 0;
	ret = malloc (sizeof (void *) * (ft_arrlen(arr) + 2));
	while (arr && arr[i])
	{
		ret[i] = arr[i];
		i++;
	}
	ret [i] = to_append;
	ret [i + 1] = NULL;
	free (arr);
	arr = ret;
	return (ret);
}

char	**ft_arrappend(char **arr, char *to_append)
{
	char	**ret;
	int		i;

	i = 0;
	ret = malloc (sizeof (char *) * (ft_arrlen((void **) arr) + 2));
	while (arr && arr[i])
	{
		ret[i] = arr[i];
		i++;
	}
		
	
	ret [i] = to_append;
	ret [i + 1] = NULL;
	free (arr);
	arr = ret;
	return (ret);
}

static t_prog	*init_prog(char *envp[])
{
	t_prog	*prog;

	prog = ft_calloc(1, sizeof(t_prog));
	if (!prog)
		exit_prog(NULL, (printf("ERR IN PROG CREATION!\n"), 1));
	prog->env = envp;
	prog->infile_fd = dup(STDIN_FILENO);
	prog->outfile_fd = dup(STDOUT_FILENO);
	return (prog);
}

static void	get_infile(int *argc, char **argv[], t_prog *prog)
{
	char	**args;

	args = *argv;
	if (ft_strncmp(args[0], "<", 1) == 0 && *argc > 1)
	{
		if (ft_strncmp(args[0], "<", 2) == 0)
		{
			prog->infile_path = args[1];
			close(prog->infile_fd);
			prog->infile_fd = -1;
			*argv += 2;
			*argc -= 2;
		}
		else if (ft_strncmp(args[0], "<<", 3) == 0)
		{
			prog->heredoc_deli = args[1];
			*argv += 2;
			*argc -= 2;
		}
		else
			exit_prog(prog, (printf("WRONG OPTION: %s\n", args[1]), 1));
	}
}

static void	get_outfile(int *argc, char **argv[], t_prog *prog)
{
	if (*argc < 2)
		return ;
	if (ft_strncmp((*argv)[(*argc) - 2], ">", 1) == 0)
	{
		prog->outfile_fd = -1;
		prog->outfile_path = (*argv)[*argc - 1];
		if (ft_strncmp((*argv)[*argc - 2], ">", 2) == 0)
			prog->outfile_permissions = (O_CREAT | O_WRONLY | O_TRUNC);
		else if (ft_strncmp((*argv)[*argc - 2], ">>", 3) == 0)
			prog->outfile_permissions = (O_CREAT | O_WRONLY | O_APPEND);
		else
			exit_prog(prog, (printf("NO BUENO: %s\n", (*argv)[*argc - 2]), 1));
		(*argv)[*argc - 2] = NULL;
	}
}

static void	get_cmds(char *argv[], t_prog *prog)
{
	int		i;
	char	**cmds;


	i = -1;
	cmds = NULL;
	while (1)
	{
		i++;
		if (!argv[i])
		{
			if (cmds)
				prog->cmds = (char ***) ft_arrappend_void((void **) prog->cmds, (void *) cmds);
			return;
		}
		if (ft_strncmp(argv[i], "|", 2) == 0)
		{
			if (cmds)
			{
				prog->cmds = (char ***) ft_arrappend_void((void **) prog->cmds, (void *) cmds);
				cmds = NULL;
			}
		}
		else
			cmds = ft_arrappend(cmds, ft_strdup(argv[i]));
	}
	prog->cmd_num = ft_arrlen((void **) prog->cmds);
}

t_prog	*prog_creation(int argc, char *argv[], char *env[])
{
	t_prog	*prog;

	prog = init_prog(env);
	argc -= 1;
	argv += 1;
	get_infile(&argc, &argv, prog);
	get_outfile(&argc, &argv, prog);
	get_cmds(argv, prog);
	return (prog);
}


int main(int argc, char *argv[], char *env[])
{
	argv = (char *[]) {"pipex", "echo", "echo1", "echo2", "echo3", "|","cat", "|","cat1", "cat2", NULL};
	argc = ft_arrlen((void **) argv);

	printf("checkpnt 1\n");
	t_prog	*prog = prog_creation(argc, argv, env);
	printf("\n\n[%lu]\n", ft_arrlen((void **) prog->cmds));
	for (int i = 0; prog->cmds[i];i++)
	{
		for (int j = 0; prog->cmds[i][j];j++)
		{
			printf ("{%s}", prog->cmds[i][j]);
		}
		printf("\n");
	}
	exit_prog(prog, 0);
	return 0;
}