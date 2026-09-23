#include "ft_ls.h"


t_args parse_args(int ac, char **av)
{
	t_args	args;
	int		i;
	int		path_count;

	args.flags = 0;
	args.paths = malloc(sizeof(char *) * (ac + 1));
	path_count = 0;
	i = 1;
	while (i < ac)
	{
		if (av[i][0] == '-')
		{
			if (ft_strchr(av[i], 'l'))
				args.flags |= l;
			if (ft_strchr(av[i], 'R'))
				args.flags |= R;
			if (ft_strchr(av[i], 'r'))
				args.flags |= r;
			if (ft_strchr(av[i], 'a'))
				args.flags |= a;
			if (ft_strchr(av[i], 't'))
				args.flags |= t;
		}
		else
			args.paths[path_count++] = av[i];
		i++;
	}
	if (path_count == 0)
		args.paths[path_count++] = ".";
	args.paths[path_count] = NULL;
	return (args);
}


int compare(const void *a, const void *b) {
	return ft_strncmp(*(char **)a, *(char **)b, SIZE_MAX);
}


void reverse_names(char **names, int count)
{
	int   i;
	char *tmp;

	i = 0;
	while (i < count / 2)
	{
		tmp = names[i];
		names[i] = names[count - 1 - i];
		names[count - 1 - i] = tmp;
		i++;
	}
}