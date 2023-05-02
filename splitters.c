
#include "pipex.h"

char	*improved_trim(char *str, char *charset)
{
	char	*temp;

	temp = ft_strtrim(str, charset);
	free (str);
	return (temp);
}

static int	split_extander(char **split)
{
	int	j;

	j = 0;
	while (++j && split[j])
	{
		split[0] = str_append (split[0], split[j], " ");
		if (ft_strchr(split[j], '\'') || !split[j + 1])
		{
			split[j] = (free (split[j]), NULL);
			return (j);
		}
		split[j] = (free (split[j]), NULL);
	}
	return (j);
}

char	**split_command(char *str, char tosplit)
{
	char	**split;
	int		i;
	int		j;

	split = ft_split(str, ' ');
	i = -1;
	while (split[++i])
	{
		if (ft_strchr(split[i], '\''))
		{
			j = split_extander(&split[i]);
			split[i] = improved_trim(split[i], "\'");
			cpy_arr(&(split[i + 1]), &(split[i + j + 1]));
		}
	}
	return (split);
}
/* char	*improved_trim(char *str, char *charset)
{
	char	*temp;

	temp = ft_strtrim(str, charset);
	free (str);
	return (temp);
}

static int	split_extander(char **split)
{
	int	j;

	j = 0;
	while (++j && split[j])
	{
		split[0] = str_append (split[0], split[j], " ");
		if (ft_strchr(split[j], '\'') || !split[j + 1])
		{
			split[j] = (free (split[j]), NULL);
			return (j);
		}
		split[j] = (free (split[j]), NULL);
	}
	return (j);
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
			j = split_extander(&split[i]);
			split[i] = improved_trim(split[i], "\'");
			cpy_arr(&(split[i + 1]), &(split[i + j + 1]));
		}
	}
	return (split);
} */