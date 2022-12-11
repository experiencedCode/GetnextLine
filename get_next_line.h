

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# define BUFF_SIZE 10000000
# include "libft.h"

int				get_next_line(const int fd, char **line);
typedef struct	s_file_buff
{
	int			fd;
	char		*start;
	char		*str;
}				t_file_buff;
#endif
