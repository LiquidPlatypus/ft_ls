#ifndef FT_LS_H
# define FT_LS_H

# include <stdio.h>
# include <sys/stat.h>
# include <dirent.h>
# include <sys/types.h>
# include <stdint.h>

# include "libft/libft.h"

enum e_bool {
	FALSE,
	TRUE
};

enum e_args {
	l = 1 << 0,
	R = 1 << 1,
	r = 1 << 2,
	a = 1 << 3,
	t = 1 << 4
};

typedef struct s_args {
	int		flags;
	char	**paths;
} t_args;

typedef struct s_entry {
	char	*name;
	char	*fullpath;
} t_entry;

t_args	parse_args(int ac, char **av);
int		compare(const void *a, const void *b);
int		compare_time(const void *a, const void *b);
void	reverse_entries(t_entry *entries, int count);

#endif //FT_LS_H
