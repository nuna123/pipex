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
