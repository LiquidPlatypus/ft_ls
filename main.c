#include "ft_ls.h"


int ls(int flags, char *path) {

	DIR *dir;
	struct dirent *entry;
	char **names;

	if ((dir = opendir(path)) == NULL) {
		perror(path);
		return 1;
	}

	int count = 0;
	while ((entry = readdir(dir)) != NULL) {
		if (entry->d_name[0] == '.' && !(flags & a))
			continue;
		count++;
	}
	rewinddir(dir);

	names = malloc(sizeof(char *) * (count + 1));

	int i = 0;
	while ((entry = readdir(dir)) != NULL) {
		if (entry->d_name[0] == '.' && !(flags & a))
			continue;
		names[i++] = ft_strdup(entry->d_name);
	}
	names[i] = NULL;
	closedir(dir);
	qsort(names, count, sizeof(char *), compare);

	int first = TRUE;
	i = 0;
	while (names[i] != NULL) {
		if (first)
			printf("%s", names[i]);
		else
			printf("  %s", names[i]);
		first = FALSE;
		i++;
	}
	printf("\n");

	i = 0;
	while (names[i] != NULL)
		free(names[i++]);
	free(names);

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
