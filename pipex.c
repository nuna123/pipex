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

void	get_outfile_fd(int i, t_prog *prog, int file_fds[2], int pipees[2])
{
	if (i == prog->cmd_num - 1)
	{
		file_fds[1] = prog->outfile_fd;
		if (prog->outfile_fd == -1)
			file_fds[1] = open (prog->outfile_path,
					prog->outfile_permissions, 0666);
		if (file_fds[1] == -1)
		{
			perror(prog->outfile_path);
			exit_prog (prog, errno);
		}
	}
	else
	{
		pipe (pipees);
		file_fds[1] = pipees[1];
	}
}

int	pipe_the_stuff(t_prog *prog)
{
	int		i;
	int		stat;
	int		file_fds[2];
	int		pipees[2];
	char	**cmd;

	i = -1;
	file_fds[0] = prog->infile_fd;
	if (prog->infile_fd == -1)
		file_fds[0] = open (prog->infile_path, O_RDONLY);
	if (file_fds[0] == -1)
	{
		perror(prog->infile_path);
		exit_prog (prog, errno);
	}
	while (++i < prog->cmd_num)
	{
		get_outfile_fd(i, prog, file_fds, pipees);
		cmd = get_full_cmd (prog, i);
		stat = piper (cmd, prog->env, file_fds[0], file_fds[1]);
		free_arr ((void **) cmd);
		close_fds((int []){file_fds[0], file_fds[1], -1});
		file_fds[0] = pipees[0];
	}
	return (stat);
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
}
/* 
int	main(int argc, char *argv[], char *env[])
{
	t_prog	*prog;

	if (argc == 1)
		exit_prog(NULL, (printf("WRONG NUM OF ARGUMENTS!\n"), 1));
	prog = prog_creation(argc, argv, env);
	if (prog->heredoc_deli)
		here_doc(prog);
	exit_prog (prog, pipe_the_stuff(prog));
	return (0);
}
 */
/*
	// TO RECIEVE ARGS AS STRING IN COMMAND LINE INSTEAD OF ONE LONG STRING
if (argc == 2)
	{
		char *s = ft_strjoin ("pipex ", argv[1]);
		argv = ft_split (s, ' ');
		free (s);
		argc = ft_arrlen((void **)argv);
		prog = prog_creation(argc, argv, env);
		if (prog->heredoc_deli)
			here_doc(prog);
		int stat = pipe_the_stuff (prog);
		for (int i = 0;argv[i]; i++)
			free(argv[i]);
		free(argv);
		exit_prog (prog, stat);
	}
*/
