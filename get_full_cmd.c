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

char	*improved_trim(char *str, char *charset)
{
	char	*temp;

	temp = ft_strtrim(str, charset);
	free (str);
	return (temp);
}

char	**split_command(char *cmd)
{
	char	**split;
	int		i;
	int		j;

	split = ft_split(cmd, ' ');
	i = -1;
	while (split[++i])
	{
		if (ft_strchr(split[i], '\''))
		{
			j = 0;
			while (++j && split[i + j])
			{
				split[i] = str_append (split[i], split[i + j], " ");
				if (ft_strchr(split[i + j], '\'') || !split[i + j + 1])
				{
					split[i + j] = (free (split[i + j]), NULL);
					break ;
				}
				split[i + j] = (free (split[i + j]), NULL);
			}
			split[i] = improved_trim(split[i], "\'");
			cpy_arr(&(split[i + 1]), &(split[i + j + 1]));
		}
	}
	return (split);
}

static void	extander(char **split_path, char **splot)
{
	int		i;
	char	*temp;
	char	*path;

	i = -1;
	while (split_path[++i])
	{
		temp = ft_strjoin(split_path[i], "/");
		path = ft_strjoin(temp, splot[0]);
		free(temp);
		if (access(path, 0) == 0)
		{
			free(splot[0]);
			splot[0] = path;
			break ;
		}
		free(path);
	}
}

char	**get_full_cmd(char *cmd, char **env)
{
	char	**splot;
	char	*path;
	char	**split_path;

	splot = split_command(cmd);
	if (ft_strchr(splot[0], '/'))
		return (splot);
	while (*env)
	{
		if (ft_strnstr(*env, "PATH", 5))
			path = *env + 5;
		env++;
	}
	split_path = ft_split(path, ':');
	extander(split_path, splot);
	free_arr((void **) split_path);
	return (splot);
}
/* 

int	main(int argc, char *argv[], char *env[])
{
	char	**fullname;

	if (argc > 1)
		fullname = get_full_cmd(argv[1], env);
	else
		fullname = get_full_cmd("ls", env);

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