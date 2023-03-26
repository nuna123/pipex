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

static void	forker(int pipees[2], int file_fds[2],
				char **cmds[2], t_prog *prog)
{
	close (pipees[0]);
	dup2(file_fds[0], STDIN_FILENO);
	dup2(pipees[1], STDOUT_FILENO);
	close (file_fds[0]);
	close (file_fds[1]);
	close (pipees[1]);
	execve(cmds[0][0], cmds[0], prog->env);
	release_cmds(cmds);
	exit_prog(prog, 127);
}

static void	forker2(int pipees[2], int file_fds[2],
				char **cmds[2], t_prog *prog)
{
	char	*l;

	close (pipees[1]);
	dup2(pipees[0], STDIN_FILENO);
	dup2(file_fds[1], STDOUT_FILENO);
	close (file_fds[0]);
	close (file_fds[1]);
	close (pipees[0]);
	if (cmds[1])
		execve(cmds[1][0], cmds[1], prog->env);
	else
	{
		l = get_next_line(STDIN_FILENO);
		while (l)
		{
			write(STDOUT_FILENO, l, ft_strlen(l));
			free(l);
			l = get_next_line(STDIN_FILENO);
		}
		release_cmds(cmds);
		exit_prog(prog, 0);
	}
	release_cmds(cmds);
	exit_prog(prog, 127);
}

static int	wait_for_child(char **cmd, pid_t child)
{
	int		stat;

	waitpid(child, &stat, 0);
	if (WEXITSTATUS(stat) == 127 && cmd)
		ft_printf("pipex: %s: %s\n", cmd[0], "command not found");
	return (WEXITSTATUS(stat));
}

int	piper(char **cmds[2], t_prog *prog,
		int file_fds[2])
{
	pid_t	child;
	int		pipees[2];
	int		stat;

	if (pipe(pipees) != 0)
		return (1);
	child = fork();
	if (child == 0)
		forker(pipees, file_fds, cmds, prog);
	stat = wait_for_child(cmds[0], child);
	close(pipees[1]);
	child = fork();
	if (child == 0)
		forker2(pipees, file_fds, cmds, prog);
	if (cmds[1])
		stat = wait_for_child(cmds[1], child);
	close_fds((int []){pipees[0], file_fds[0], file_fds[1], -1});
	release_cmds(cmds);
	return (stat);
}
