/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/03/27 19:37:27 by gbourgeo          #+#    #+#             */
/*   Updated: 2017/03/03 03:42:53 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_shell_log.h"
#include "ft_shell.h"

#include <stdlib.h>
#include <unistd.h>

static int ft_dup2(int oldd, int newd)
{
    if (newd != oldd)
        close(newd);
    return (dup2(oldd, newd));
}

void ft_pipe_pipe(t_args *pipes, t_shell *e)
{
    int   fildes[2];
    pid_t pid;

    if (pipe(fildes) != -1)
    {
        pid = fork();
        if (pid == 0)
        {
            ft_dup2(fildes[1], 1);
            close(fildes[0]);
            exit(check_and_exec((const char **) pipes->args, e));
        }
        else if (pid > 0)
        {
            ft_dup2(fildes[0], 0);
            close(fildes[1]);
            waitpid(pid, &e->status, 0);
            e->status = check_and_exec((const char **) pipes->next->args, e);
        }
        else
            ft_shell_log(SH_LOG_LEVEL_WARN, "fork() failed");
    }
    else
        ft_shell_log(SH_LOG_LEVEL_WARN, "pipe() failed");
}
