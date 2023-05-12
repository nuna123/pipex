/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nroth <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 16:48:27 by nroth             #+#    #+#             */
/*   Updated: 2023/02/15 16:48:28 by nroth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_arr(void **arr)
{
	int	i;

	i = -1;
	while (arr[++i])
		free(arr[i]);
	free(arr);
}

int	release_cmds(char **cmds[2])
{
	if (cmds[0])
		free_arr((void **) cmds[0]);
	if (cmds[1])
		free_arr((void **) cmds[1]);
	cmds[0] = NULL;
	cmds[1] = NULL;
	return (1);
}

//TERMINATE ARR WITH -1
void	close_fds(int arr[])
{
	int	i;

	i = 0;
	while (arr[i] != -1)
	{
		close(arr[i]);
		i++;
	}
}

void	cpy_arr(char **dest, char **src)
{
	while (*src)
	{
		*dest = *src;
		dest++;
		src++;
	}
	*dest = NULL;
}

void	exit_prog(t_prog *prog, int exitstat)
{
	int	i;

	i = 0;
	if (prog)
	{
		if (prog->cmds)
		{
/* 			for (int i = 0; prog->cmds[i];i++)
			{
				for (int j = 0; prog->cmds[i][j];j++)
				{
					free(prog->cmds[i][j]);
				}
			free(prog->cmds[i]);
			} */
			while (prog->cmds[i])
				free_arr((void **) prog->cmds[i++]);
			free (prog->cmds);
		}

		free (prog);
	}
	exit (exitstat);
}
