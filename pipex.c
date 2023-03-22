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

int	figure_fd1(int args[3], char *argv[], int pipees[2])
{
	int	res;

	res = -1;
	if (args[0] != args[1] - 2 && args[0] != args[1] - 3 && args[1] > 4)
		res = pipees[1];
	else
	{
		if (args[2] > -1)
			res = open(argv[args[1] - 1],
					O_CREAT | O_WRONLY | O_APPEND, 0666);
		else
			res = open(argv[args[1] - 1],
					O_CREAT | O_WRONLY | O_TRUNC, 0666);
	}
	return (res);
}

void	pipe_the_stuff(int argc, char *argv[], char *env[], int in_fd)
{
	int		file_fds[2];
	char	**cmds[2];
	int		pipees[2];
	int		i;
	int		stat;

	i = 2;
	cmds[1] = NULL;
	while (i < argc - 1)
	{
		cmds[0] = get_full_cmd(argv[i], env);
		if (i + 1 < argc - 1)
			cmds[1] = get_full_cmd(argv[i + 1], env);
		file_fds[0] = pipees[0];
		if (i == 2 && in_fd != -1)
			file_fds[0] = in_fd;
		else if (i == 2)
			file_fds[0] = open(argv[1], O_RDONLY);
		pipe(pipees);
		file_fds[1] = figure_fd1((int []){i, argc, in_fd}, argv, pipees);
		stat = piper(cmds, env, file_fds);
		i += 2;
	}
	exit (stat);
}

void	read_input(char *del, int out_fd)
{
	char	*line;
	int		done;

	done = 0;
	ft_printf("heredoc>");
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
		if (done)
			break ;
		ft_printf("heredoc>");
		line = get_next_line(STDIN_FILENO);
	}
	close(out_fd);
}

int	main(int argc, char *argv[], char *env[])
{
	int	pipees[2];
	int	infile;

	if (argc < 4)
	{
		ft_printf("Invalid number of arguments.\n");
		return (1);
	}
	if (ft_strncmp(argv[1], "here_doc", 8) == 0)
	{
		if (pipe(pipees) == -1)
			return (-1);
		read_input(argv[2], pipees[1]);
		pipe_the_stuff(argc - 1, &argv[1], env, pipees[0]);
	}
	infile = open(argv[1], O_RDONLY);
	if (infile == -1)
		exit((perror(argv[1]), 2));
	close(infile);
	pipe_the_stuff(argc, argv, env, -1);
	return (0);
}

/*
	(void) argc;
	(void) argv;
	pipe_the_stuff(4,
		(char *[]) {"here_doc", "LIM" ,"wc", "catsa", "test.txt"}, env);
*/

/* 
		int	pipees[2];
	int	infile;

	if (argc < 4)
	{
		ft_printf("Invalid number of arguments.\n");
		return (1);
	}
	if (ft_strncmp(argv[1], "here_doc", 8) == 0)
	{
		if (pipe(pipees) == -1)
			return (-1);
		read_input(argv[2], pipees[1]);
		pipe_the_stuff(argc - 1, &argv[1], env, pipees[0]);
	}
	infile = open(argv[1], O_RDONLY);
	if (infile == -1)
		exit((perror(argv[1]), 2));
	close(infile);
	pipe_the_stuff(argc, argv, env, -1);
*/
