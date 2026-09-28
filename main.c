#include "ft_ls.h"


int ls(int flags, char *path) {

	DIR				*dir;
	struct dirent	*entry;
	t_entry			*entries;
	int				count;
	int				i;
	char			*tmp;

	if ((dir = opendir(path)) == NULL) {
		perror(path);
		return 1;
	}

	count = 0;
	while ((entry = readdir(dir)) != NULL) {
		if (entry->d_name[0] == '.' && !(flags & a))
			continue;
		count++;
	}
	rewinddir(dir);

	entries = malloc(sizeof(t_entry) * (count + 1));

	i = 0;
	while ((entry = readdir(dir)) != NULL) {
		if (entry->d_name[0] == '.' && !(flags & a))
			continue;
		entries[i].name = ft_strdup(entry->d_name);
		tmp = ft_strjoin(path, "/");
		entries[i].fullpath = ft_strjoin(tmp, entry->d_name);
		free(tmp);
		i++;
	}
	closedir(dir);
	entries[i].name = NULL;
	entries[i].fullpath = NULL;
	if (flags & t)
		qsort(entries, count, sizeof(t_entry), compare_time);
	else
		qsort(entries, count, sizeof(t_entry), compare);
	if (flags & r)
		reverse_entries(entries, count);

	if (flags & R)
		printf("%s:\n", path);
	i = 0;
	while (entries[i].name != NULL) {
		if (i == 0)
			printf("%s", entries[i].name);
		else
			printf("  %s", entries[i].name);
		i++;
	}
	printf("\n");

	if (flags & R) {
		for (i = 0; entries[i].name != NULL; i++) {
			if (!ft_strncmp(".", entries[i].name, SIZE_MAX) || !ft_strncmp("..", entries[i].name, SIZE_MAX))
				continue;
			struct stat st;
			if (lstat(entries[i].fullpath, &st) == 0 && S_ISDIR(st.st_mode)) {
				printf("\n");
				ls(flags, entries[i].fullpath);
			}
		}
	}

	i = 0;
	while (entries[i].name != NULL) {
		free(entries[i].name);
		free(entries[i].fullpath);
		i++;
	}
	free(entries);
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
