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
	char	*temp;
	char	*cmds;

	i = -1;
	cmds = ft_strdup("");
	while (argv[++i])
	{
		temp = ft_strjoin(cmds, argv[i]);
		cmds = (free (cmds), temp);
		if (argv[i + 1])
		{
			temp = ft_strjoin(cmds, " ");
			cmds = (free (cmds), temp);
		}
	}
	prog->cmds = ft_split(cmds, '|');
	free(cmds);
	i = -1;
	while (prog->cmds[++i])
	{
		temp = ft_strtrim(prog->cmds[i], " ");
		prog->cmds[i] = (free (prog->cmds[i]), temp);
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
