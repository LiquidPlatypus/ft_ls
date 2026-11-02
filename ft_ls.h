#ifndef FT_LS_H
# define FT_LS_H

# include <stdio.h>
# include <sys/stat.h>
# include <dirent.h>
# include <sys/types.h>
# include <stdint.h>
# include <pwd.h>
# include <grp.h>
# include <time.h>

# include "libft/libft.h"

// 6 months in seconds, for printing conversion
# define SIX_MONTHS 15778476

// Basic boolean
enum e_bool {
	FALSE,
	TRUE
};

// All supported flags
# define VALID_OPTIONS "lRrat"

// Bits for flags
enum e_args {
	l = 1 << 0,
	R = 1 << 1,
	r = 1 << 2,
	a = 1 << 3,
	t = 1 << 4
};

// Struct regrouping flags and args
typedef struct s_args {
	int		flags;
	char	**paths;
} t_args;

// Struct regrouping name and full path for each entry
typedef struct s_entry {
	char	*name;
	char	*fullpath;
} t_entry;

t_args	parse_args(int ac, char **av);
int		compare(const void *a, const void *b);
int		compare_paths(const void *a, const void *b);
int		compare_time(const void *a, const void *b);
void	reverse_entries(t_entry *entries, int count);
void	print_entries(t_entry *entries, int flags);
char	*quote_name(const char *name);

#endif //FT_LS_H
