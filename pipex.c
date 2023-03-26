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
// int	figure_fd1(int args[3], char *argv[], int pipees[2])
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

// void	pipe_the_stuff(int argc, char *argv[], char *env[], int in_fd)
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
		cmds[0] = get_full_cmd(prog->cmds[i], prog->env);
		if (i + 1 < prog->cmd_num)
			cmds[1] = get_full_cmd(prog->cmds[i + 1], prog->env);
		file_fds[0] = pipees[0];
		if (i == 0 && prog->infile_fd != -1)
			file_fds[0] = prog->infile_fd;
		else if (i == 0)
			file_fds[0] = open(prog->infile_path, O_RDONLY);
		pipe(pipees);
		file_fds[1] = figure_fd1(prog, i, pipees);
		stat = piper(cmds, prog->env, file_fds);
		i += 2;
	}
	exit (stat);
}
/* 
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
 */
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

void	here_doc(t_prog *prog)
{
	int	pipees[2];

	if (pipe(pipees) == -1)
		exit (1);

	read_input(prog->heredoc_deli, pipees[1]);
	prog->infile_fd = pipees[0];
	pipe_the_stuff(prog);
}

t_prog	*init_prog(char *envp[])
{
	t_prog *prog;
	prog = ft_calloc(1, sizeof(t_prog));

	prog->env = envp;
	prog->infile_fd = STDIN_FILENO;
	prog->outfile_fd = STDOUT_FILENO;
	return (prog);
}

void	exit_prog(t_prog *prog, int exitstat)
{
	free (prog);
	exit (exitstat);
}

int	main(int argc, char *argv[], char *env[])
{

	t_prog *prog;
	prog = init_prog(env);

/* 	int argc = 10;
	char **argv = ((char *[]){"prog", "<<", "DEL", "cat -e" ,"|", "wc", "|", "cat", ">", "outfile", NULL}); */
	
	int i;
	if (argc == 1)
	{
		argv = ft_split("./a.out << DEL cat", ' ');
		argc = ft_arrlen((void **) argv);

		// exit_prog(prog, (printf("WRONG NUM OF ARGUMENTS!\n"), 1));

	}
	else if (argc == 2)
	{
		argv = ft_split (argv[1], ' ');
		argc = ft_arrlen((void **) argv);
		//prog->cmds = &argv[1];
		//PIPE t_prog
	}
	printf("cmds num: %i\n", argc);

	i = 0;
	 while (argv[i])
	 	printf("\t%s\n", argv[i++]);


	argc -= 1;
	argv += 1;
	if (ft_strncmp(argv[0], "<", 1) == 0)
	{
		if (ft_strncmp(argv[0], "<", 2) == 0)
		{
			prog->infile_path = argv[1];
			prog->infile_fd = -1;
			argv += 2;
			argc -= 2;
		}
		else if (ft_strncmp(argv[0], "<<", 3) == 0)
		{
			prog->heredoc_deli = argv[1];
			argv += 2;
			argc -= 2;
		}
		else
			exit_prog(prog, (printf("WRONG OPTION: %s\n", argv[1]), 1));
	}
	if (ft_strncmp(argv[argc - 2], ">", 1) == 0)
	{
		prog->outfile_path = argv[argc - 1];
		if (ft_strncmp(argv[argc - 2], ">", 2) == 0)
			prog->outfile_permissions = (O_CREAT | O_WRONLY | O_TRUNC);
		else if (ft_strncmp(argv[argc - 2], ">>", 3) == 0)
			prog->outfile_permissions = (O_CREAT | O_WRONLY | O_APPEND);
		else
			exit_prog(prog, (printf("WRONG OPTION: %s\n", argv[argc - 2]), 1));
		argv[argc - 2] = NULL;
	}

	i = 1;
	int j;

	while (argv[i])
	{
		if (ft_strncmp(argv[i], "|", 2) == 0)
		{
			j = i - 1;
			while (argv[++j])
				argv[j] = argv[j + 1];
		}
		else
			i++;
	}
	prog->cmds = argv;
	prog->cmd_num = i;

	printf("cmds num: %i\n", prog->cmd_num);
	ft_printf("\ncmds:");
	i = 0;
	 while (prog->cmds[i])
	 	printf("\t%s\n", prog->cmds[i++]);
	

	if (prog->heredoc_deli)
		here_doc(prog);
	pipe_the_stuff(prog);

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
		exit_prog(prog, (perror(argv[1]), 2));
	close(infile);
	pipe_the_stuff(argc, argv, env, -1);
*/





/* int	main(int argc, char *argv[], char *env[])
{
	int	infile;

	if (argc < 5)
	{
		ft_printf("Invalid number of arguments.\n");
		return (1);
	}
	if (ft_strncmp(argv[1], "here_doc", 8) == 0)
		here_doc (argc, argv, env);
	infile = open(argv[1], O_RDONLY);
	if (infile == -1)
		exit_prog(prog, (perror(argv[1]), 2));
	close(infile);
	pipe_the_stuff(argc, argv, env, -1);
	return (0);
}
 */
