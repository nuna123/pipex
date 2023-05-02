/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nroth <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 16:25:25 by nroth             #+#    #+#             */
/*   Updated: 2023/05/02 16:25:27 by nroth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	handle_echo(char **command, int out_fd)
{
	int	i;

	i = 0;
	if (ft_arrlen((void **)command) == 1)
		return (ft_putstr_fd("\n", out_fd), 0);
	if (!ft_strncmp(command[1], "-n", 3))
		i = 1;
	while (command[++i])
	{
		ft_putstr_fd(command[i], out_fd);
		if (i < (int) ft_arrlen((void **)command) - 1 && command[i][0] != '\0')
			ft_putstr_fd(" ", out_fd);
	}
	if (ft_strncmp(command[1], "-n", 3))
		ft_putstr_fd("\n", out_fd);
	return (0);
}

int	print_pwd(int out_fd)
{
	char	pwd[10000];

	getcwd(pwd, 10000);
	ft_putstr_fd (pwd, out_fd);
	ft_putstr_fd ("\n", out_fd);
	return (0);
}

/*

int main(void)
{
	handle_echo (ft_split("echo hello -n ", ' '), STDOUT_FILENO);
} */
