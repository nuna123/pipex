/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_full_name.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nroth <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 12:10:46 by nroth             #+#    #+#             */
/*   Updated: 2023/02/14 12:10:47 by nroth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static char	*gfc_extander(char **split_path, char *cmd)
{
	int		i;
	char	*temp;
	char	*path;

	i = -1;
	while (split_path[++i])
	{
		temp = ft_strjoin(split_path[i], "/");
		path = ft_strjoin(temp, cmd);
		free(temp);
		if (access(path, 0) == 0)
		{
			errno = 0;
			return (path);
		}
		free(path);
	}
	return (ft_strdup(cmd));
}

char	*get_full_cmd(t_prog *prog, char *cmd)
{
	char	*path;
	char	**split_path;

	if (ft_strchr(cmd, '/')
		|| !ft_strncmp(cmd, "echo", 5)
		|| !ft_strncmp(cmd, "pwd", 4))
		return (ft_strdup(cmd));
	path = getenv("PATH");
	if (!path)
	{
		ft_putstr_fd(PROG_NAME, STDERR_FILENO);
		ft_putstr_fd(cmd, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(strerror(2), STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		exit_prog(prog, 127);
	}
	split_path = ft_split(path, ':');
	cmd = gfc_extander(split_path, cmd);
	ft_arrfree((void **) split_path);
	return (cmd);
}

/*
int	main(int argc, char *argv[], char *env[])
{
	char	**fullname;

	if (argc > 1)
		fullname = get_full_cmd(argv[1]);
	else
		fullname = get_full_cmd("ls");
	ft_printf("FULLNAME: \n");
	for (int i = 0;fullname[i]; i++)
	{
		ft_printf("%s, ", fullname[i]);
		free(fullname[i]);
	}
	free(fullname);
	return (0);
}
 */