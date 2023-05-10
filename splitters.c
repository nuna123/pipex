/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   splitters.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nroth <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 10:55:22 by nroth             #+#    #+#             */
/*   Updated: 2023/05/10 10:55:24 by nroth            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

//ADDING ALL ARGV TO ONE INPUT LINE
char	*stringize(char *argv[])
{
	char	*temp;
	char	*cmds;
	int		i;

	i = -1;
	cmds = ft_strdup("");
	while (argv[++i])
	{
		temp = ft_strjoin(cmds, argv[i]);
		cmds = (free (cmds), temp);
		if (argv[i + 1])
		{
			temp = ft_strjoin(cmds, " ");
			cmds = (free (cmds), temp);
		}
	}
	return (cmds);
}

char	*expand_arg(char *str, int counter)
{
	char	*argname;
	char	*arg;
	int		argname_len;
	char	*newstr;

	argname_len = 0;
	while (!ft_strchr("\'\" ", str[counter + argname_len + 1]))
		argname_len++;
	argname = ft_substr (str, counter + 1, argname_len);
	arg = getenv(argname);
	newstr = ft_calloc((ft_strlen(str) - argname_len)
			+ ft_strlen(arg), sizeof(char));
	ft_strlcpy(newstr, str, counter);
	if (arg)
		ft_strlcpy(ft_strchr(newstr, 0), arg, ft_strlen(arg) + 1);
	else
		counter++;
	ft_strlcpy(ft_strchr(newstr, 0),
		str + counter + argname_len + 1, ft_strlen(str) + 1);
	free(argname);
	return (newstr);
}

void	split_extand(int *is_quotes, char *str, int *counter)
{
	if ((str[*counter] == '\"' || str[*counter] == '\'')
		&& !*is_quotes)
	{
		if (str[*counter - 1] && str[*counter - 1] == '\\')
			printf ("CHAR: {%c}\n", str[*counter - 1]);
		else
		{
			*is_quotes = 2;
			if (str[*counter] == '\'')
				*is_quotes = 1;
			ft_memmove(&str[*counter],
				&str[*counter + 1], ft_strlen(&(str[*counter])));
			*counter -= 1;
		}
	}
	else if (((str[*counter] == '\"' && *is_quotes == 2)
			|| (str[*counter] == '\'' && *is_quotes == 1)))
	{
		ft_memmove(&str[*counter], &str[*counter + 1],
			ft_strlen(&(str[*counter + 1])) + 1);
		*is_quotes = 0;
		*counter -= 1;
	}
}

//IS_QUOTES: 0 == NO, 1 == SINGLE QUOTES, 2 == DOUBLE
char	**split_string(char *str)
{
	char	**ret;
	int		counter;
	int		is_quotes;

	is_quotes = 0;
	ret = ft_calloc(1, sizeof (char *));
	counter = -1;
	while (str[++counter])
	{
		if (str[counter] == '$' && is_quotes != 1)
			str = expand_arg(str, counter);
		if (str[counter] == ' ' && !is_quotes)
		{
			ret = ft_arrappend(ret, ft_substr(str, 0, counter));
			str = &(str[counter]) + 1;
			counter = -1;
			is_quotes = 0;
		}
		else
			split_extand(&is_quotes, str, &counter);
	}
	if (*str)
		ret = ft_arrappend(ret, ft_strdup(str));
	return (ret);
}

char	**resplit_argv(int argc, char *argv[])
{
	char	*str;
	char	**ret;

	if (argc < 2)
		return (argv);
	str = stringize (argv + 1);
	ret = split_string (str);
	free (str);
	return (ret);
}

/* 
int main (int argc, char **argv)
{

	char *str;

	if (argc > 1)
		str = stringize (argv + 1);
	else
		str = ft_strdup("H E \\ \\\' O");

	printf("STR: \n\t[%s]\n", str);
	char **s = split_string (str);
	free (str);
	for (int i = 0;s[i];i++)
	{
		printf ("{%s}\n", s[i]);
		free (s[i]);
	}
	free (s);
	return 0;
}
 */