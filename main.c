#include "ft_ls.h"


int ls(int flags, char *path, int path_count) {

	DIR				*dir;
	struct dirent	*entry;
	t_entry			*entries;
	int				count;
	int				i;
	char			*tmp;


	struct stat st;
	int lstat_result = lstat(path, &st);
	int saved_errno = errno; // Save errno in case lstat fails
	if (lstat_result == 0 && !S_ISDIR(st.st_mode)) {
		t_entry single[2];
		single[0].name = ft_strdup(path);
		single[0].fullpath = ft_strdup(path);
		single[1].name = NULL;
		single[1].fullpath = NULL;
		print_entries(single, flags);
		free(single[0].name);
		free(single[0].fullpath);
		return 0;
	}

	if ((dir = opendir(path)) == NULL) {
		if (lstat_result == -1) {
			// If lstat failed, print the error message for the path.
			fprintf(stderr, "ls: cannot access '%s': %s\n", path, strerror(saved_errno));
		} else {
			// If lstat succeeded but opendir failed, print a generic error message.
			fprintf(stderr, "ls: cannot open directory '%s': %s\n", path, strerror(errno));
		}
		return 1;
	}

	// Count the number of files in the directory.
	count = 0;
	while ((entry = readdir(dir)) != NULL) {
		if (entry->d_name[0] == '.' && !(flags & a))
			continue;
		count++;
	}
	rewinddir(dir);

	entries = malloc(sizeof(t_entry) * (count + 1));

	// Read the directory entries into the array.
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
	// Sort the entries based on the flags.
	if (flags & t)
		qsort(entries, count, sizeof(t_entry), compare_time);
	else
		qsort(entries, count, sizeof(t_entry), compare);
	if (flags & r)
		reverse_entries(entries, count);

	// Header shown with -R, or when more than one path was given on the command line.
	if ((flags & R) || path_count > 1) {
		char *h = quote_name(path);
		printf("%s:\n", h);
		free(h);
	}

	if (flags & l) {
		// Print the total blocks for the directory.
		int total_blocks = 0;
		for (i = 0; entries[i].name != NULL; i++) {
			struct stat st;
			if (lstat(entries[i].fullpath, &st) == -1) {
				perror(entries[i].fullpath);
				continue;
			}
			total_blocks += st.st_blocks;
		}
		printf("total %d\n", total_blocks / 2);
	}

	// Display the entries based on the flags.
	print_entries(entries, flags);

	// If the -R flag is set, recursively list subdirectories.
	if (flags & R) {
		for (i = 0; entries[i].name != NULL; i++) {
			if (!ft_strncmp(".", entries[i].name, SIZE_MAX) || !ft_strncmp("..", entries[i].name, SIZE_MAX))
				continue;
			struct stat st;
			// lstat (not stat) so symlinks are never followed, avoiding infinite recursion.
			if (lstat(entries[i].fullpath, &st) == 0 && S_ISDIR(st.st_mode)) {
				printf("\n");
				ls(flags, entries[i].fullpath, path_count);
			}
		}
	}

	// Free allocated memory for entries.
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

	// Count how many paths were given, to decide whether headers are needed.
	int path_count = 0;
	while (args.paths[path_count] != NULL)
		path_count++;
	qsort(args.paths, path_count, sizeof(char *), compare_paths);

	int i = 0;
	int exit_code = 0;
	while (args.paths[i] != NULL) {
		// Blank line between the listings of two different paths, like the real ls.
		if (i > 0)
			printf("\n");
		if (ls(args.flags, args.paths[i], path_count))
			exit_code = 1;
		i++;
	}

	free(args.paths);
	return exit_code;
}
