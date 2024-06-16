/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe_rights.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/03/27 19:39:54 by gbourgeo          #+#    #+#             */
/*   Updated: 2014/03/29 20:44:27 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

void        ft_pipe_right(t_args *pipes, t_shell *e)
{
    int     fd;
    char    *path;

    path = ft_str2join(getcwd(NULL, 0), "/", pipes->next->args[0]);
    if ((fd = open(path, O_RDWR | O_CREAT | O_TRUNC, 0644)) == -1)
    {
        ft_error(e, "no such file or directory: %s", pipes->next->args[0]);
        e->status = 1;
    }
    else
    {
        ft_dup2(fd, 1);
        e->status = check_and_exec((const char **)pipes->args, e);
    }
    close(fd);
    free(path);
}

void        ft_pipe_right_2(t_args *pipes, t_shell *e)
{
    int     fd;
    char    *path;

    path = ft_str2join(getcwd(NULL, 0), "/", pipes->next->args[0]);
    if ((fd = open(path, O_RDWR | O_CREAT | O_APPEND, 0644)) == -1)
    {
        ft_error(e, "no such file or directory: %s", pipes->next->args[0]);
        e->status = 1;
    }
    else
    {
        ft_dup2(fd, 1);
        e->status = check_and_exec((const char **)pipes->args, e);
    }
    close(fd);
    free(path);
}
