/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fork.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/01/22 19:03:03 by gbourgeo          #+#    #+#             */
/*   Updated: 2014/04/29 18:26:16 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_shell.h"
#include "ft_shell_builtins.h"
#include "ft_shell_constants.h"
#include "ft_shell_log.h"
#include "libft.h"
#include <stddef.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

static char *check_path(char *path)
{
    struct stat buffer;

    if (access(path, F_OK) == 0 && stat(path, &buffer) == 0)
    {
        return (path);
    }
    free(path);
    return (NULL);
}

static char *get_path_from_given_path(const char *cmd, t_shell *shell)
{
    char *path = NULL;
    char *tmp  = NULL;

    if (*cmd == '.')
    {
        tmp  = ft_strjoin(ft_getenv("PWD", shell), "/");
        path = ft_get_path(ft_strjoin(tmp, cmd));
        free(tmp);
        return (path);
    }
    tmp  = ft_strjoin(ft_getenv("HOME", shell), ft_strchr(cmd, '/'));
    path = ft_get_path(tmp);
    free(tmp);
    return (path);
}

static char *get_path_from(const char *cmd, t_shell *shell)
{
    char **path = NULL;
    char  *tmp  = NULL;
    char  *ret  = NULL;
    int    iter = 0;

    if (*cmd == '/')
    {
        return (check_path(ft_strdup(cmd)));
    }
    if (*cmd == '.' || *cmd == '~')
    {
        return (check_path(get_path_from_given_path(cmd, shell)));
    }
    path = ft_strsplit(ft_getenv("PATH", shell), ':');
    if (path == NULL)
    {
        path = ft_strsplit(shell->bin_path, ':');
    }
    if (path == NULL)
    {
        return (NULL);
    }
    while (path[iter] && ret == NULL)
    {
        tmp = ft_strjoin(path[iter], "/");
        ret = check_path(ft_strjoin(tmp, cmd));
        free(tmp);
        iter++;
    }
    ft_freetab(&path);
    return (ret);
}

retcode_e fork_function(const char **args, t_shell *shell)
{
    char     *path   = NULL;
    pid_t     pid    = 0;
    retcode_e status = SHELL_STATUS_FATAL;

    path = get_path_from(args[0], shell);
    if (path == NULL)
    {
        ft_shell_log(SH_LOG_LEVEL_WARN, "command not found: %s", args[0]);
        return (SHELL_STATUS_FATAL);
    }
    pid = fork();
    if (pid > 0)
    {
        if (waitpid(pid, (int *) &status, 0) != pid)
        {
            ft_shell_log(SH_LOG_LEVEL_WARN, "Waitpid error");
        }
    }
    else if (pid == 0)
    {
        execve(path, (char * const *) args, shell->global_env);
        ft_shell_log(SH_LOG_LEVEL_WARN, "Wrong argument: %s", args[0]);
    }
    else
    {
        ft_shell_log(SH_LOG_LEVEL_WARN, "Fork error");
    }
    free(path);
    return (status);
}
