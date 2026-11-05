#include "ft_ls.h"

/**
 * @brief parse the args and flags
 * @param ac number of args
 * @param av list of args
 * @return flags and args
 */
t_args parse_args(int ac, char **av)
{
	t_args	args;
	int		i = 1;
	int		j;
	int		path_count = 0;
	int		is_after_doublecarret = 0;

	args.flags = 0;
	args.paths = malloc(sizeof(char *) * (ac + 1));
	while (i < ac)
	{
		j = 1;
		if (!is_after_doublecarret && ft_strncmp(av[i], "--", SIZE_MAX) == 0) {
			is_after_doublecarret = 1;
			i++;
			continue;
		}
		if (!is_after_doublecarret && av[i][0] == '-' && av[i][1] != '\0')
		{
			while (av[i][j]) {
				if (!ft_strchr(VALID_OPTIONS, av[i][j])) {
					fprintf(stderr, "ls: invalid option -- '%c'\n", av[i][j]);
					fprintf(stderr, "Try 'ls --help' for more information.\n");
					free(args.paths);
					exit(2);
				}
				j++;
			}
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


/**
 * @brief compare entry based on ASCII
 * @param a first element
 * @param b second element
 * @return result of comparison
 */
int compare(const void *a, const void *b) {
	t_entry *ea;
	t_entry *eb;

	ea = (t_entry *)a;
	eb = (t_entry *)b;
	return ft_strncmp(ea->name, eb->name, SIZE_MAX);
}


/**
 * @brief compare paths based on ASCII
 * @param a first element
 * @param b second element
 * @return result of comparison
 */
int compare_paths(const void *a, const void *b) {
	return ft_strncmp(*(char **)a, *(char **)b, SIZE_MAX);
}


/**
 * @brief compare time of two elements
 * @param a first element
 * @param b second element
 * @return bool to know which element is older/newer
 */
int compare_time(const void *a, const void *b) {
	t_entry	*ea = (t_entry *)a;
	t_entry	*eb = (t_entry *)b;
	struct stat sa;
	struct stat sb;

	lstat(ea->fullpath, &sa);
	lstat(eb->fullpath, &sb);

	if (sa.st_mtime == sb.st_mtime) {
		if (sa.st_mtim.tv_nsec == sb.st_mtim.tv_nsec)
			return ft_strncmp(ea->name, eb->name, SIZE_MAX);
		return (sa.st_mtim.tv_nsec < sb.st_mtim.tv_nsec ? 1 : (sa.st_mtim.tv_nsec > sb.st_mtim.tv_nsec ? -1 : 0));
	}
	return (sa.st_mtime < sb.st_mtime ? 1 : (sa.st_mtime > sb.st_mtime ? -1 : 0));
}


/**
 * @brief reverse list of paths
 * @param entries all paths
 * @param count how many paths
 */
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


/**
 * @brief perform the printing logic
 * @param entries all paths
 * @param flags flags
 */
void print_entries(t_entry *entries, int flags)
{
	int i;

	if (flags & l)
	{
		time_t now = time(NULL);
		i = 0;
		while (entries[i].name != NULL)
		{
			struct stat		st;
			char			perms[11];
			int				masks[9] = {S_IRUSR, S_IWUSR, S_IXUSR, S_IRGRP, S_IWGRP, S_IXGRP, S_IROTH, S_IWOTH, S_IXOTH};
			char			letters[3] = {'r', 'w', 'x'};
			int				j;
			struct passwd	*pwd;
			struct group	*grp;
			struct tm		*tm_info;
			char			target[PATH_MAX];
			char			*display_name;

			if (lstat(entries[i].fullpath, &st) == -1) {
				perror(entries[i].fullpath);
				i++;
				continue;
			}
			display_name = NULL;
			if (S_ISDIR(st.st_mode))
				perms[0] = 'd';
			else if (S_ISLNK(st.st_mode)) {
				perms[0] = 'l';
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
			else if (S_ISCHR(st.st_mode))
				perms[0] = 'c';
			else if (S_ISBLK(st.st_mode))
				perms[0] = 'b';
			else if (S_ISFIFO(st.st_mode))
				perms[0] = 'p';
			else if (S_ISSOCK(st.st_mode))
				perms[0] = 's';
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
			if (st.st_mode & S_ISVTX)
				perms[9] = (perms[9] == 'x') ? 't' : 'T';
			if (st.st_mode & S_ISUID)
				perms[3] = (perms[3] == 'x') ? 's' : 'S';
			if (st.st_mode & S_ISGID)
				perms[6] = (perms[6] == 'x') ? 's' : 'S';
			perms[10] = '\0';

			pwd = getpwuid(st.st_uid);
			char *user_name = pwd ? pwd->pw_name : ft_itoa(st.st_uid);
			grp = getgrgid(st.st_gid);
			char *group_name = grp ? grp->gr_name : ft_itoa(st.st_gid);

			tm_info = localtime(&st.st_mtime);
			char buffer[20];
			if (st.st_mtime < now - SIX_MONTHS || st.st_mtime > now)
				strftime(buffer, sizeof(buffer), "%b %e  %Y", tm_info);
			else
				strftime(buffer, sizeof(buffer), "%b %e %H:%M", tm_info);

			if (!display_name)
				display_name = quote_name(entries[i].name);

			printf("%s %ld %s %s %ld %s %s\n", perms, st.st_nlink, user_name,
									group_name, st.st_size, buffer, display_name);
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
			char *display_name;

			display_name = quote_name(entries[i].name);
			if (i == 0)
				printf("%s", display_name);
			else
			free(display_name);
			i++;
		}
		if (i > 0)
			printf("\n");
	}
}


/**
 * @brief put quote around name if needed
 * @param name name
 * @return quoted name
 */
char *quote_name(const char *name)
{
	char *tmp;
	char *quoted_name;
	char *quote;

	if (!ft_strchr(name, ' ') && !ft_strchr(name, '\'')
		&& !ft_strchr(name, '"'))
		return (ft_strdup(name));
	if (ft_strchr(name, '\''))
		quote = "\"";
	else
		quote = "'";
	tmp = ft_strjoin(quote, name);
	quoted_name = ft_strjoin(tmp, quote);
	free(tmp);
	return (quoted_name);
}