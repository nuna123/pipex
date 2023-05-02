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

int	piper(char **cmd, char *env[],
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
