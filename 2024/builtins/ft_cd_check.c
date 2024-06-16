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

#include "ft_minishell.h"

static char     *check_if_good_env(const char *dir, const char *cmd_name, t_shell *e)
{
    if (!dir || !ft_strcmp(dir, "~"))
    {
        if (ft_getenv("HOME", e->env) && !dir)
            return (ft_strdup(ft_getenv("HOME", e->env)));
        else if (ft_getenv("HOME", e->env) && !ft_strcmp(dir, "~"))
            return (ft_strjoin(ft_getenv("HOME", e->env), &dir[1]));
        ft_error(e, "%s: HOME not defined", cmd_name);
        return (ft_strdup(ft_getenv("PWD", e->env)));
    }
    else if (ft_strcmp(dir, "-") == 0)
    {
        if (ft_getenv("OLDPWD", e->env))
        {
            ft_putendl(ft_getenv("OLDPWD", e->env));
            return (ft_strdup(ft_getenv("OLDPWD", e->env)));
        }
        ft_error(e, "%s: OLDPWD not defined", cmd_name);
        return (ft_strdup(ft_getenv("PWD", e->env)));
    }
    return (NULL);
}

char            *cd_check(const char *dir, const char *cmd_name, void *e)
{
    char    *pwd;
    char    *tmp;

    if ((pwd = check_if_good_env(dir, cmd_name, e)))
        return (ft_get_path(pwd));
    else if (dir[0] == '/')
        pwd = ft_strdup(dir);
    else
    {
        tmp = ft_strjoin(ft_getenv("PWD", ((t_shell *)e)->env), "/");
        pwd = ft_strjoin(tmp, dir);
        free(tmp);
    }
    return (ft_get_path(pwd));
}
