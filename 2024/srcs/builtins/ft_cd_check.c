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

#include "ft_builtins.h"
#include "ft_log.h"
#include "libft.h"

static char *cd_check_special_arg(const char *dirname, const char *cmdname, t_shell *shell)
{
    const char *ptr;

    if (dirname == NULL || ft_strcmp(dirname, "~") == 0)
    {
        ptr = ft_getenv("HOME", shell);
        if (ptr != NULL)
            return (ft_strjoin(ptr, dirname + 1));
        ft_log(SH_LOG_LEVEL_WARN, "%s: HOME not defined", cmdname);
        return ((char *) -1);
    }
    else if (ft_strcmp(dirname, "-") == 0)
    {
        ptr = ft_getenv("OLDPWD", shell);
        if (ptr != NULL)
        {
            ft_putendl(ptr);
            return (ft_strdup(ptr));
        }
        ft_log(SH_LOG_LEVEL_WARN, "%s: OLDPWD not defined", cmdname);
        return ((char *) -1);
    }
    return (NULL);
}

char *cd_check(const char *dirname, const char *cmdname, t_shell *shell)
{
    char *pwd;
    char *tmp;

    pwd = cd_check_special_arg(dirname, cmdname, shell);
    if (pwd == (void *) -1)
        return (NULL);
    else if (pwd == NULL)
    {
        if (dirname[0] == '/')
            pwd = ft_strdup(dirname);
        else
        {
            tmp = ft_strjoin(ft_getenv("PWD", shell), "/");
            pwd = ft_strjoin(tmp, dirname);
            free(tmp);
        }
    }
    return (ft_get_path(pwd));
}
