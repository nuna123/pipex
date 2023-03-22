/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nroth <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 16:50:06 by nroth             #+#    #+#             */
/*   Updated: 2023/02/15 16:50:08 by nroth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "Libft/libft.h"

# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <errno.h>

//COLORS
# define NRM  "\x1B[0m"
# define RED  "\x1B[0m"
// # define RED  "\x1B[31m"
# define GRN  "\x1B[32m"
# define YEL  "\x1B[33m"
# define BLU  "\x1B[34m"
# define MAG  "\x1B[35m"
# define CYN  "\x1B[36m"
# define WHT  "\x1B[37m"


typedef struct s_prog
{
	char	*infile_path; // if not defind, default is STDIN
	char	*outfile_path; // if not defind, default is STDOUT

	int		outfile_permissions;

	char	**cmds;
	char	*heredoc_deli;
}	t_prog;

// GET_FULL_NAME
char	**get_full_cmd(char *cmd, char **env);

//PIPER
int		piper(char **cmds[2], char *env[],
			int file_fds[2]);
//PIPEX_UTILS
void	free_arr(void **arr);
int		release_cmds(char **cmds[2]);
void	close_fds(int arr[]);
void	cpy_arr(char **dest, char **src);
char	**split_command(char *cmd);
#endif
