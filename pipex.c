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
/* 
void	printeeer(char **cmd1, char **cmd2, char *infile_path, char *outfile_path)
{
	int	i;

	if (infile_path){
		ft_printf("infile: {%s}\n", infile_path);
	}
	if (outfile_path){
		ft_printf("outfile: {%s}\n", outfile_path);
	}
	if (cmd1)
	{
		ft_printf("\ncmd1:");
		i = 0;
		while (cmd1[i])
			ft_printf("%s, ", cmd1[i++]);
	}
	if (cmd2)
	{
		ft_printf("\ncmd2:");
		i = 0;
		while (cmd2[i])
			ft_printf("%s, ", cmd2[i++]);
	}
	ft_printf("\n-----------------------------------------\n\n");
}

 */

int	figure_fd1(t_prog *prog, int i, int pipees[2])
{
	int	res;

	res = -1;
	if (i != prog->cmd_num - 1 && i != prog->cmd_num - 2)
		res = pipees[1];
	else if (prog->outfile_path)
		res = open(prog->outfile_path, prog->outfile_permissions, 0666);
	else
		res = prog->outfile_fd;
	return (res);
}

void	pipe_the_stuff(t_prog *prog)
{
	int		file_fds[2];
	char	**cmds[2];
	int		pipees[2];
	int		i;
	int		stat;

	i = 0;
	cmds[1] = NULL;
	while (i < prog->cmd_num)
	{
		cmds[0] = get_full_cmd(prog, i);
		if (i + 1 < prog->cmd_num)
			cmds[1] = get_full_cmd(prog, i + 1);
		file_fds[0] = pipees[0];
		if (i == 0 && prog->infile_fd != -1)
			file_fds[0] = prog->infile_fd;
		else if (i == 0)
			file_fds[0] = open(prog->infile_path, O_RDONLY);
		pipe(pipees);
		file_fds[1] = figure_fd1(prog, i, pipees);
		stat = piper(cmds, prog, file_fds);
		i += 2;
	}
	exit (stat);
}

void	read_input(char *del, int out_fd)
{
	char	*line;

	ft_printf("heredoc> ");
	line = get_next_line(STDIN_FILENO);
	while (line)
	{
		if (ft_strncmp(line, del, ft_strlen(del)) == 0
			&& line[ft_strlen(del)] == '\n')
		{
			free (line);
			break ;
		}
		write(out_fd, line, ft_strlen(line));
		free(line);
		ft_printf("heredoc> ");
		line = get_next_line(STDIN_FILENO);
	}
	close(out_fd);
}

void	here_doc(t_prog *prog)
{
	int	pipees[2];

	if (pipe(pipees) == -1)
		exit (1);
	read_input(prog->heredoc_deli, pipees[1]);
	close (prog->infile_fd);
	prog->infile_fd = pipees[0];
	pipe_the_stuff(prog);
}

/* void pipex (int argc, char *argv[], char *env[])
{
	t_prog	*prog;

	// if (argc == 1)
	// 	exit_prog(NULL, (printf("WRONG NUM OF ARGUMENTS!\n"), 1));
	prog = prog_creation(argc, argv, env);
	if (prog->heredoc_deli)
		here_doc(prog);
	pipe_the_stuff(prog);
} */

int	main(int argc, char *argv[], char *env[])
{
	t_prog	*prog;

	if (argc == 1)
		exit_prog(NULL, (printf("WRONG NUM OF ARGUMENTS!\n"), 1));
	prog = prog_creation(argc, argv, env);
	if (prog->heredoc_deli)
		here_doc(prog);
	pipe_the_stuff(prog);
	return (0);
}
/*
	printf("-------------------\nARGC: %i\n", argc);
	ft_printf("\nARGV:");
	int i = 0;
	 while (argv[i])
	 	printf("\t%s\n", argv[i++]);
	printf("-------------------\n");
*/

/*
	printf("cmds num: %i\n", prog->cmd_num);
	ft_printf("\ncmds:");
	i = 0;
	 while (prog->cmds[i])
	 	printf("\t%s\n", prog->cmds[i++]);
*/

/*
	else if (argc == 2)
	{
		argv = ft_split (argv[1], ' ');
		argc = ft_arrlen((void **) argv);
	}
*/
