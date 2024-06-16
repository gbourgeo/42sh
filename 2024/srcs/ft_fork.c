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

#include "ft_minishell.h"

static char     *check_path(char *path)
{
    struct stat buffer;

    if (access(path, F_OK) == 0 && stat(path, &buffer) == 0)
        return (path);
    free(path);
    return (NULL);
}

static char     *get_path_from_given_path(const char *cmd, char * const *env)
{
    char        *path;
    char        *tmp;

    tmp = NULL;
    if (*cmd == '.')
    {
        tmp = ft_strjoin(ft_getenv("PWD", env), "/");
        path = ft_get_path(ft_strjoin(tmp, cmd));
        free(tmp);
        return (path);
    }
    tmp = ft_strjoin(ft_getenv("HOME", env), ft_strchr(cmd, '/'));
    path = ft_get_path(tmp);
    free(tmp);
    return (path);
}

static char     *get_path_from(const char *cmd, const char *base_path, char * const *env)
{
    int         i;
    char        **path;
    char        *tmp;
    char        *ret;

    i = 0;
    ret = NULL;
    tmp = NULL;
    if (*cmd == '/')
        return (check_path(ft_strdup(cmd)));
    if (*cmd == '.' || *cmd == '~')
        return (check_path(get_path_from_given_path(cmd, env)));
    if ((path = ft_strsplit(ft_getenv("PATH", env), ':')) == NULL)
        path = ft_strsplit(base_path, ':');
    if (path == NULL)
        return (NULL);
    while (path[i] && ret == NULL)
    {
        tmp = ft_strjoin(path[i], "/");
        ret = check_path(ft_strjoin(tmp, cmd));
        free(tmp);
        i++;
    }
    ft_freetab(&path);
    return (ret);
}

int             fork_function(const char **args, t_shell *e)
{
    pid_t       pid;
    int         status;
    char        *path;

    status = 127;
    if ((path = get_path_from(args[0], e->bin_path, e->env)) == NULL)
    {
        ft_error(e, "command not found: %s", args[0]);
        return (127);
    }
    pid = fork();
    if (pid > 0)
    {
        if (waitpid(pid, &status, 0) != pid)
            ft_error(e, "Waitpid error");
    }
    else if (pid == 0)
    {
        execve(path, (char * const *)args, e->env);
        ft_error(e, "Wrong argument: %s", args[0]);
    }
    else
        ft_error(e, "Fork error");
    free(path);
    return (status);
}
