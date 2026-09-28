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
	t_entry *ea;
	t_entry *eb;

	ea = (t_entry *)a;
	eb = (t_entry *)b;
	return ft_strncmp(ea->name, eb->name, SIZE_MAX);
}


int compare_time(const void *a, const void *b) {
	t_entry	*ea = (t_entry *)a;
	t_entry	*eb = (t_entry *)b;
	struct stat sa;
	struct stat sb;

	stat(ea->fullpath, &sa);
	stat(eb->fullpath, &sb);

	if (sa.st_mtime == sb.st_mtime) {
		if (sa.st_mtim.tv_nsec == sb.st_mtim.tv_nsec)
			return ft_strncmp(ea->name, eb->name, SIZE_MAX);
		return (sa.st_mtim.tv_nsec < sb.st_mtim.tv_nsec ? 1 : (sa.st_mtim.tv_nsec > sb.st_mtim.tv_nsec ? -1 : 0));
	}
	return (sa.st_mtime < sb.st_mtime ? 1 : (sa.st_mtime > sb.st_mtime ? -1 : 0));
}


void reverse_entries(t_entry *entries, int count)
{
	int   i;
	t_entry tmp;

	i = 0;
	while (i < count / 2)
	{
		tmp = entries[i];
		entries[i] = entries[count - 1 - i];
		entries[count - 1 - i] = tmp;
		i++;
	}
}