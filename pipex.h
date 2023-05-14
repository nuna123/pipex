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

# include "../Libft/libft.h"

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

# define PROG_NAME "pipex: "

typedef struct s_prog
{
	char	*infile_path;
	char	*outfile_path;

	int		infile_fd;
	int		outfile_fd;

	int		outfile_permissions;

	char	***cmds;
	int		cmd_num;

	char	*heredoc_deli;
	char	**env;
}	t_prog;

// GET_FULL_CMD
// static char	*gfc_extander(char **split_path, char *cmd);
char		*get_full_cmd(t_prog *prog, char *cmd);

//PIPER
// int	forker(char **cmd, char *env[], int in_fd, int out_fd);

int			piper(char **cmd, char *env[],
				int in_fd, int out_fd);

// static void	get_outfile_fd(int i, t_prog *prog,
//	int file_fds[2], int pipees[2]);
int			pipe_the_stuff(t_prog *prog);

//PIPEX_UTILS
void		close_fds(int arr[]);
void		exit_prog(t_prog *prog, int exitstat);
void		read_input(char *del, int out_fd);

//COMMANDS
int			handle_echo(char **command, int out_fd);
int			print_pwd(int out_fd);

//PROG_CREATION
// static t_prog	*init_prog(char *envp[]);
// static void		get_infile(int argc, char *argv[], t_prog *prog);
// static void		get_outfile(int argc, char *argv[], t_prog *prog);
// static void		get_cmds(char *argv[], t_prog *prog);
t_prog		*prog_creation(int argc, char *argv[], char *env[]);

#endif
