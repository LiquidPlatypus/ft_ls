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

int ls(int flags, char *path) {

	DIR *dir;
	struct dirent *entry;

	if ((dir = opendir(path)) == NULL) {
		perror(path);
		return 1;
	}

	int first = TRUE;
	while ((entry = readdir(dir)) != NULL) {
		if (entry->d_name[0] == '.' && !(flags & a))
			continue;
		if (first == TRUE)
			printf("%s", entry->d_name);
		else
			printf("  %s", entry->d_name);
		first = FALSE;
	}
	printf("\n");
	closedir(dir);
	return 0;
}

int main(int ac, char **av) {

	t_args args = parse_args(ac, av);

	int i = 0;
	int exit_code = 0;
	while (args.paths[i] != NULL) {
		exit_code = exit_code || ls(args.flags, args.paths[i++]);
	}

	return exit_code;
}
