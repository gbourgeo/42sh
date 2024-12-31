/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe_left.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/03/27 19:42:34 by gbourgeo          #+#    #+#             */
/*   Updated: 2014/03/30 00:53:39 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_log.h"
#include "ft_shell.h"
#include "libft.h"
#include "libft2.h"

#include <fcntl.h>
#include <unistd.h>

static void ft_pipe_more(t_shell *e)
{
    const char *args[2];

    args[0]   = "more";
    args[1]   = NULL;
    e->status = check_and_exec(args, e);
}

void ft_pipe_left(t_args *pipes, t_shell *e)
{
    int   fd;
    char *path;

    path = ft_str2join(getcwd(NULL, 0), "/", pipes->next->args[0]);
    if ((fd = open(path, O_RDONLY)) == -1)
    {
        ft_log(SH_LOG_LEVEL_WARN, "no such file or directory: %s", pipes->next->args[0]);
        e->status = 1;
    }
    else
    {
        ft_dup2(fd, 0);
        if (!*pipes->args)
            ft_pipe_more(e);
        else
            e->status = check_and_exec((const char **) pipes->args, e);
    }
    close(fd);
    free(path);
}
