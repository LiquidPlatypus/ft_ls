#include "ft_ls.h"


int ls(int flags, char *path, int path_count) {

	DIR				*dir;
	struct dirent	*entry;
	t_entry			*entries;
	int				count;
	int				i;
	char			*tmp;
	struct passwd	*pwd;
	struct group	*grp;
	struct tm		*tm_info;

	char			target[PATH_MAX];
	char			*display_name;

	if ((dir = opendir(path)) == NULL) {
		char *tmp1 = ft_strjoin("ls: cannot access '", path);
		char *tmp2 = ft_strjoin(tmp1, "'");
		perror(tmp2);
		free(tmp1);
		free(tmp2);
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
	if ((flags & R) || path_count > 1)
		printf("%s:\n", path);

	// Display the entries based on the flags.
	if (flags & l) {
		i = 0;
		int total_blocks = 0;
		// Calculate total blocks for the directory.
		while (entries[i].name != NULL) {
			struct stat st;
			if (lstat(entries[i].fullpath, &st) == -1) {
				perror(entries[i].fullpath);
				i++;
				continue;
			}
			total_blocks += st.st_blocks;
			i++;
		}
		printf("total %d\n", total_blocks / 2);

		// Display the entries in long format.
		i = 0;
		while (entries[i].name != NULL)
		{
			struct stat st;
			char perms[11];
			// Define the permission masks and corresponding letters for user, group, and others.
			int masks[9] = {S_IRUSR, S_IWUSR, S_IXUSR, S_IRGRP, S_IWGRP, S_IXGRP, S_IROTH, S_IWOTH, S_IXOTH};
			char letters[3] = {'r', 'w', 'x'};
			int j;
			// Get the file status using lstat.
			if (lstat(entries[i].fullpath, &st) == -1) {
				perror(entries[i].fullpath);
				i++;
				continue;
			}
			display_name = NULL;
			// Determine the file type and set the first character of the permissions string accordingly.
			if (S_ISDIR(st.st_mode))
				perms[0] = 'd';
			else if (S_ISLNK(st.st_mode)) {
				perms[0] = 'l';
				// Read the symlink target to append "-> target" to the name.
				ssize_t len = readlink(entries[i].fullpath, target, sizeof(target) - 1);
				if (len == -1) {
					perror(entries[i].fullpath);
				} else {
					target[len] = '\0';
					char *tmp2 = ft_strjoin(entries[i].name, " -> ");
					display_name = ft_strjoin(tmp2, target);
					free(tmp2);
				}
			}
			else
				perms[0] = '-';
			// Set the permission bits for the file.
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

			// Get the user and group names.
			pwd = getpwuid(st.st_uid);
			char *user_name = pwd ? pwd->pw_name : ft_itoa(st.st_uid);
			grp = getgrgid(st.st_gid);
			char *group_name = grp ? grp->gr_name : ft_itoa(st.st_gid);

			tm_info = localtime(&st.st_mtime);
			char buffer[20];
			strftime(buffer, sizeof(buffer), "%b %e %H:%M", tm_info);

			// Not a symlink (or readlink failed): display_name is just the name.
			if (!display_name)
				display_name = ft_strdup(entries[i].name);

			printf("%s %ld %s %s %ld %s %s\n", perms, st.st_nlink, user_name,
											group_name, st.st_size, buffer, display_name);
			// Only free user_name/group_name if they came from ft_itoa (pwd/grp was NULL).
			if (!pwd)
				free(user_name);
			if (!grp)
				free(group_name);
			free(display_name);
			i++;
		}
	}
	else {
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
		exit_code = exit_code || ls(args.flags, args.paths[i], path_count);
		i++;
	}

	return exit_code;
}
