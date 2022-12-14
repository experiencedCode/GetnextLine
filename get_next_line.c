

#include "get_next_line.h"
#include <unistd.h>

static t_list		*create_node(int fd)
{
	t_list		*ret;
	t_file_buff *cont;

	cont = (t_file_buff *)ft_memalloc(sizeof(t_file_buff));
	cont->fd = fd;
	cont->str = ft_strnew(BUFF_SIZE);
	cont->start = cont->str;
	ret = ft_lstnew(cont, sizeof(t_file_buff));
	return (ret);
}

static t_file_buff	*get_extra(int fd, t_list **extra_list)
{
	t_list *temp;

	if (!extra_list || !*extra_list)
		*extra_list = create_node(fd);
	temp = *extra_list;
	while (temp)
	{
		if (((t_file_buff *)temp->content)->fd == fd)
			return ((t_file_buff *)temp->content);
		temp = temp->next;
	}
	ft_lstadd(extra_list, create_node(fd));
	return ((t_file_buff *)(*extra_list)->content);
}

static void			handle_extra(t_file_buff *extra, char **ret, char **end)
{
	long	dist;
	char	*loc;

	loc = ft_strchr(extra->str, '\n');
	if (loc)
	{
		dist = (long)(loc - extra->str);
		*end = (char *)1;
		extra->str[dist] = 0;
		*ret = ft_strdup(extra->str);
		extra->str += dist + 1;
	}
	else
	{
		*ret = ft_strdup(extra->str);
		ft_strclr(extra->start);
		extra->str = extra->start;
	}
}

static char			*handle_data(int count, char **ret, \
		char *buff, t_file_buff *extra)
{
	long	dist;
	char	*loc;
	int		len;

	len = ft_strlen(*ret);
	loc = ft_strchr(buff, '\n');
	if (!loc)
	{
		*ret = (char *)ft_realloc((void *)*ret, len, len + count + 1);
		ft_strncat(*ret, buff, count);
		ft_strclr(buff);
	}
	else
	{
		dist = (long)(loc - buff);
		*ret = (char *)ft_realloc((void *)*ret, len, len + dist + 1);
		ft_strncat(*ret, buff, dist);
		ft_strncpy(extra->start, buff + dist + 1, BUFF_SIZE - dist - 1);
	}
	return (ft_strchr(buff, '\n'));
}

int					get_next_line(const int fd, char **line)
{
	static t_list	*extra_list;
	char			*ret;
	int				count;
	char			*tmps[2];
	t_file_buff		*extra;

	if (!line)
		return (-1);
	extra = get_extra(fd, &extra_list);
	tmps[1] = 0;
	handle_extra(extra, &ret, &tmps[1]);
	if (!tmps[1])
		tmps[0] = ft_strnew(BUFF_SIZE);
	else
		return ((*line = ret) ? 1 : 0);
	while (!tmps[1] && (count = read(fd, tmps[0], BUFF_SIZE)) > 0)
		tmps[1] = handle_data(count, &ret, tmps[0], extra);
	if (tmps[1] > tmps[0])
		ft_strcpy(extra->start, tmps[1] + 1);
	*line = ret;
	ft_strdel(&tmps[0]);
	if (tmps[1] || ret[0] || count)
		return ((count >= 0 ? 1 : -1));
	return (0);
}
