
#include "pipex.h"

//ADDING ALL ARGV TO ONE INPUT LINE
char *stringize(char *argv[])
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

char *expand_arg(char *str, int counter)
{
	char	*argname;
	char	*arg;
	int		argname_len;
	char	*newstr;

	argname_len = 0;
	while (!ft_strchr("\'\" ", str[counter + argname_len + 1]))
		argname_len++;
	printf ("argname len: %i\n", argname_len);
	argname = ft_substr (str, counter + 1, argname_len);
	arg = getenv(argname);

	printf("arg: {%s}\n", arg);
	printf("one\n");
	newstr = ft_calloc((ft_strlen(str) - argname_len) + ft_strlen(arg) ,sizeof(char));
	printf("two\n");

	ft_strlcpy(newstr, str, counter);
	printf("threee\n");

	ft_strlcpy(ft_strchr(newstr, 0), arg, ft_strlen(arg) + 1);
		printf("four\n");

	ft_strlcpy(ft_strchr(newstr, 0), str + counter + argname_len, ft_strlen(str) + 1);
		printf("five\n");

printf("newstr: {%s}\n", newstr);


	printf("len: %i ; argname: {%s}\n",argname_len, argname);
	free(argname);
	return (NULL);
}

void	split_extand (int *is_quotes, char *str, int *counter)
{
	if ((str[*counter] == '\"' || str[*counter] == '\'')
			&& !*is_quotes)
	{
		ft_memmove(&str[*counter], &str[*counter + 1], ft_strlen(&(str[*counter])));
		*is_quotes = 2;
		if (str[*counter] == '\'')
			*is_quotes = 1;
		*counter -= 1;
	}
	else if ((str[*counter] == '\"' && *is_quotes == 2)
		|| (str[*counter] == '\'' && *is_quotes == 1))
	{
		ft_memmove(&str[*counter], &str[*counter + 1], ft_strlen(&(str[*counter + 1])) + 1);
		*is_quotes = 0;
		*counter -= 1;
	}
}

char **split_string(char *str)
{
	char	**ret;
	int		counter;
	int		is_quotes; //0 == NO, 1 == SINGLE QUOTES, 2 == DOUBLE

	is_quotes = 0;
	ret = ft_calloc(1, sizeof (char *));
	counter = -1;
	while (str[++counter])
	{
		printf ("char: %c ; i: %i\n", str[counter], counter);
		if (str[counter] == '$' && is_quotes != 1)
			expand_arg(str, counter);
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
/* 

char **resplit (char *argv[])
{
	char	*cmds;
	char	**ret;
	int		counter;

	int		is_quotes; //0 == NO, 1 == SINGLE QUTES, 2 == DOUBLE 
	is_quotes = 0;

	cmds = stringize(argv);
	printf("ONE\n");

	ret = split_string (cmds);
	printf("TWO\n");

	for (int i = 0; ret[i]; i++)
		printf("{%s}\n", ret[i]);

	free(cmds);
	return (NULL);
	
}
 */
int main (int argc, char **argv)
{

	(void)argc;
	(void)argv;

	char **s = split_string ("HE\"AG\"LLO");
	for (;*s;s++)
		printf ("{%s}\n", *s);

	// printf ("\n{%s}\n", getenv("ARG"));


	return 0;
}
