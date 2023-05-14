/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piper.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nroth <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 15:03:24 by nroth             #+#    #+#             */
/*   Updated: 2023/02/14 15:03:26 by nroth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	forker(char **cmd, char *env[],
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

int	piper(char **cmd, char *env[],
		int in_fd, int out_fd)
{
	if (ft_strncmp(cmd[0], "echo", 5) == 0)
		return (handle_echo(cmd, out_fd));
	if (ft_strncmp(cmd[0], "pwd", 4) == 0)
		return (print_pwd(out_fd));
	return (forker(cmd, env, in_fd, out_fd));
}

static void	get_outfile_fd(int i, t_prog *prog, int file_fds[2], int pipees[2])
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
		cmd = prog->cmds[i];
		stat = piper (cmd, prog->env, file_fds[0], file_fds[1]);
		close_fds((int []){file_fds[0], file_fds[1], -1});
		file_fds[0] = pipees[0];
	}
	return (stat);
}
