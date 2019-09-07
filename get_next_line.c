/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rnureeva <rnureeva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/25 09:46:28 by rnureeva          #+#    #+#             */
/*   Updated: 2019/09/06 14:41:20 by rnureeva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>
#include <fcntl.h>

int				pointer(char **saver, char **str, char **line, const int fd)
{
	char			*p;

	p = ft_strchr(str[fd], '\n');
	if (*str[fd] != '\0')
	{
		if (p == NULL)
		{
			*saver = ft_strdup(str[fd]);
			free(str[fd]);
			str[fd] = NULL;
		}
		else if (p != NULL)
		{
			*p = '\0';
			*saver = ft_strdup(str[fd]);
			p++;
			ft_strcpy(str[fd], p);
			*line = *saver;
			return (1);
		}
	}
	return (0);
}

void			savbuf(char **saver, char buffer[])
{
	char			*tmp;

	if (*saver != NULL)
	{
		tmp = *saver;
		*saver = ft_strjoin(*saver, buffer);
		free(tmp);
	}
	else
		*saver = ft_strdup(buffer);
}

int				resread(char **saver, char buffer[],
						char **str, const int fd)
{
	char *p;

	p = ft_strchr(buffer, '\n');
	if (p != NULL)
	{
		*p = '\0';
		p++;
		str[fd] = ft_strdup(p);
		savbuf(saver, buffer);
		return (1);
	}
	savbuf(saver, buffer);
	return (0);
}

int				valid_args(const int fd, char **line)
{
	char		buffer[1];

	if (fd < 0 || !line || (read(fd, buffer, 0) < 0) ||
			fd > FD_MAX || BUFF_SIZE <= 0)
		return (0);
	return (1);
}

int				get_next_line(const int fd, char **line)
{
	char			buffer[BUFF_SIZE + 1];
	int				res;
	static char		*str[FD_MAX];
	char			*saver;

	if (!valid_args(fd, line))
		return (-1);
	saver = NULL;
	if ((str[fd] != NULL) && (pointer(&saver, str, line, fd) == 1))
		return (1);
	res = 1;
	while (res)
	{
		if ((res = read(fd, buffer, BUFF_SIZE)) < 0)
			return (-1);
		else if (res == 0)
			return ((*line = saver) == NULL) ? 0 : 1;
		buffer[res] = '\0';
		if (resread(&saver, buffer, str, fd))
		{
			*line = saver;
			return (1);
		}
	}
	return (1);
}
int		main(int argc, char **argv)
{
	char	*line;
	int		fd;
	int		ret;

	if (argc == 2)
		fd = open(argv[1], O_RDONLY);
	else
		fd = 0;
	line = NULL;
	ret = 1;
	while (ret)
	{
		ret = get_next_line(fd, &line);
		printf("%s\treturn value - %d\n", line, ret);
		ft_strdel(&line);
	}
	close(fd);
//	while (1);  //for check on leaks (leaks a.out)
}