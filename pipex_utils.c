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

void	exit_prog(t_prog *prog, int exitstat)
{
	int	i;

	i = 0;
	if (prog)
	{
		if (prog->cmds)
		{
			while (prog->cmds[i])
				ft_arrfree((void **) prog->cmds[i++]);
			free (prog->cmds);
		}
		free (prog);
	}
	exit (exitstat);
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
