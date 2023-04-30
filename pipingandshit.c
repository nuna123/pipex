/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipingandshit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nroth <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/30 14:48:11 by nroth             #+#    #+#             */
/*   Updated: 2023/04/30 14:48:13 by nroth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
/* **************************************************************************
	runs one command based on the arr [cmd],
	in which cmd[0] is the full path of the executable
	the exit codes follow bash's exit codes,
	in which 126 = permission denied, 127 = command not found
 */

int	pipering(char **cmd, char *env[],
		int in_fd, int out_fd)
{
	pid_t	child;
	int		stat;

	child = fork();
	if (child == 0)
	{
		dup2(in_fd, STDIN_FILENO);
		dup2(out_fd, STDOUT_FILENO);
		execve(cmd[0], cmd, env);
		perror (cmd[0]);
		if (errno == 2)
			errno = 127;
		if (errno == 13)
			errno = 126;
		exit (errno);
	}
	else
		waitpid (child, &stat, 0);
	return (WEXITSTATUS(stat));
}

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

int	redirect(t_prog *prog)
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
		stat = pipering (cmd, prog->env, file_fds[0], file_fds[1]);
		free_arr ((void **) cmd);
		close_fds((int []){file_fds[0], file_fds[1], -1});
		file_fds[0] = pipees[0];
	}
	return (stat);
}

/*
int	main(int i, char **a, char *env[])
{
	char *argv[] = {"pipex", "<", "infile", "/usr/bin/cat -e","|",
			 "/usr/bin/csdfat -e", ">", "outfile", NULL};
	int argc = ft_arrlen((void **) argv);

	t_prog *prog = prog_creation(argc, argv, env);

	int x = redirect (prog);

	printf ("EXIT CODE: %i\n\n", x);

	exit_prog (prog, x);
	return (x);
}
 */