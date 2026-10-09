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

	if (flags & l)
	{
		i = 0;
		while (entries[i].name != NULL)
		{
			struct stat st;
			char perms[11];
			int masks[9] = {S_IRUSR, S_IWUSR, S_IXUSR, S_IRGRP, S_IWGRP, S_IXGRP, S_IROTH, S_IWOTH, S_IXOTH};
			char letters[3] = {'r', 'w', 'x'};
			int j;

			if (lstat(entries[i].fullpath, &st) == -1) {
				perror(entries[i].fullpath);
				i++;
				continue;
			}
			if (S_ISDIR(st.st_mode))
				perms[0] = 'd';
			else if (S_ISLNK(st.st_mode))
				perms[0] = 'l';
			else
				perms[0] = '-';
			j = 0;
			while (j < 9)
			{
				if (st.st_mode & masks[j])
					perms[j + 1] = letters[j % 3];
				else
					perms[j + 1] = '-';
				j++;
			}
			perms[10] = '\0';
			printf("%s %s\n", perms, entries[i].name);
			i++;
		}
	}
	else
	{
		i = 0;
		while (entries[i].name != NULL) {
			if (i == 0)
				printf("%s", entries[i].name);
			else
				printf("  %s", entries[i].name);
			i++;
		}
		printf("\n");
	}

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
