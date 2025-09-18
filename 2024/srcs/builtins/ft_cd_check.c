/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/02/26 09:56:58 by gbourgeo          #+#    #+#             */
/*   Updated: 2014/03/27 19:15:15 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_shell.h"
#include "ft_shell_builtins.h"
#include "ft_shell_log.h"
#include "libft.h"
#include <stddef.h>
#include <stdlib.h>

static int cd_check_special_arg(
    char      **pwd,
    const char *dirname,
    const char *cmdname,
    t_shell    *shell)
{
    const char *ptr = NULL;

    *pwd = NULL;
    if (dirname == NULL || ft_strcmp(dirname, "~") == 0)
    {
        ptr = ft_getenv("HOME", shell);
        if (ptr != NULL)
        {
            *pwd = ft_strjoin(ptr, dirname + 1);
            return (0);
        }
        ft_log(SH_LOG_LEVEL_WARN, "%s: HOME not defined", cmdname);
        return (-1);
    }
    if (ft_strcmp(dirname, "-") == 0)
    {
        ptr = ft_getenv("OLDPWD", shell);
        if (ptr != NULL)
        {
            ft_putendl(ptr);
            *pwd = ft_strdup(ptr);
            return (0);
        }
        ft_log(SH_LOG_LEVEL_WARN, "%s: OLDPWD not defined", cmdname);
        return (-1);
    }
    return (0);
}

char *cd_check(const char *dirname, const char *cmdname, t_shell *shell)
{
    char *pwd = NULL;
    char *tmp = NULL;

    if (cd_check_special_arg(&pwd, dirname, cmdname, shell) != 0)
    {
        return (NULL);
    }
    if (pwd == NULL)
    {
        if (dirname[0] == '/')
        {
            pwd = ft_strdup(dirname);
        }
        else
        {
            tmp = ft_strjoin(ft_getenv("PWD", shell), "/");
            pwd = ft_strjoin(tmp, dirname);
            free(tmp);
        }
    }
    return (ft_get_path(pwd));
}
