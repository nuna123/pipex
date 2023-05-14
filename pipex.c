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

static void	check_operators(char *argv[], char *env[])
{
	int		i;
	int		stat;
	char	*str;

	i = -1;
	while (argv[++i])
	{
		if (ft_strnstr("&&||", argv[i], 4) && ft_strlen (argv[i]) == 2)
		{
			str = argv[i];
			argv[i] = NULL;
			argv[0] = "./pipex";
			stat = piper(argv, env, STDIN_FILENO, STDOUT_FILENO);
			if ((stat == 0 && !ft_strncmp(str, "&&", 3))
				|| (stat != 0 && !ft_strncmp(str, "||", 3)))
			{
				argv[i] = "./pipex";
				execve("./pipex", &argv[i], env);
			}
			else
				exit (stat);
		}
	}
}

static void	here_doc(t_prog *prog)
{
	int		pipees[2];

	if (pipe(pipees) == -1)
		exit (1);
	read_input(prog->heredoc_deli, pipees[1]);
	close (prog->infile_fd);
	prog->infile_fd = pipees[0];
}

int	main(int argc, char *argv[], char *env[])
{
	t_prog	*prog;
	int		pipees[2];

	if (argc == 1)
		exit_prog(NULL, (printf("WRONG NUM OF ARGUMENTS!\n"), 1));
	check_operators(argv, env);
	prog = prog_creation(argc, argv, env);
	if (prog->heredoc_deli)
		here_doc(prog);
	exit_prog (prog, pipe_the_stuff(prog));
	return (0);
}
