/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/01/21 00:00:31 by gbourgeo          #+#    #+#             */
/*   Updated: 2014/03/27 19:14:37 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_cd.h"
#include "ft_minishell.h"

static int      ft_cd_chdir_error(char *pwd, const char *dir, const char *cmd_name, t_shell *e)
{
    struct stat buffer;
    int         ret;

    if (pwd)
    {
        ret = stat(pwd, &buffer);
        if (ret == -1)
        {
            if (lstat(pwd, &buffer) != -1)
                ft_error(e, "%s: too many levels of symbolic links: %s", cmd_name, dir);
            else
                ft_error(e, "%s: no such file or directory: %s", cmd_name, dir);
        }
        else if (!S_ISDIR(buffer.st_mode))
            ft_error(e, "%s: not a directory: %s", cmd_name, dir);
        else
            ft_error(e, "%s: permission denied: %s", cmd_name, dir);
        free(pwd);
    }
    return (1);
}

static int      cd_write_in_pwd(cd_e option, const char *dir, const char *cmd_name, t_shell *e)
{
    char        *pwd;
    char        *tmp;

    pwd = cd_check(dir, cmd_name, e);
    if (chdir(pwd) != -1)
    {
        tmp = pwd;
        if (option & CD_RESOLVE_BEFORE)
        {
            pwd = ft_getcwd(tmp, e);
            free(tmp);
        }
        cd_change_pwds(pwd, e);
        free(pwd);
        return (0);
    }
    return (ft_cd_chdir_error(pwd, dir, cmd_name, e));
}

static char     *cd_change_in_pwd(const char **dirs, const char *found, const char *pwd)
{
    char        *tmp;
    char        *tmp_2;

    tmp = ft_strndup(pwd, ft_strlen(pwd) - ft_strlen(found));
    tmp_2 = ft_strjoin(tmp, dirs[0]);
    free(tmp);
    tmp = ft_strjoin(tmp_2, found + ft_strlen(dirs[1]));
    free(tmp_2);
    return (tmp);
}

static int      cd_search_in_pwd(const char **dirs, const char *arg_name, t_shell *e)
{
    char        *pwd;
    char        *tmp;

    pwd = (char *)ft_getenv("PWD", e->env);
    if ((tmp = ft_strstr(pwd, dirs[0])) == NULL)
    {
        ft_error(e, "%s: string not in pwd: %s", arg_name, dirs[0]);
        return (1);
    }
    pwd = cd_change_in_pwd(dirs, tmp, pwd);
    if (chdir(pwd) != -1)
    {
        cd_change_pwds(pwd, e);
        ft_putendl(pwd);
        free(pwd);
        return (0);
    }
    return (ft_cd_chdir_error(pwd, dirs[0], arg_name, e));
}

static int      ft_cd_invalid_option(char option, const char *cmd_name, t_shell *e)
{
    ft_error(e, "%s: invalid option: -%c", cmd_name, option);
    ft_error(e, "usage: %s [-L|-P] [dir]", cmd_name);
    return (1);
}

int             ft_cd(const char **args, void *e)
{
    cd_e    option;
    int     i;
    int     j;

    option = CD_RESOLVE_UNDEFINED;
    i = 1;
    while (args[i] && args[i][0] == '-' && args[i][1] && !(option & CD_BREAK))
    {
        j = 1;
        while (args[i][j] && !(option & CD_BREAK))
        {
            if (ft_strcmp(args[i], "--") == 0)
                option |= CD_BREAK;
            else if (args[i][j] == 'P')
                option = CD_RESOLVE_BEFORE;
            else if (args[i][j] == 'L')
                option = CD_RESOLVE_AFTER;
            else
                return (ft_cd_invalid_option(args[i][j], args[0], e));
            j++;
        }
        i++;
    }
    if (!args[i] || !args[i + 1])
        return (cd_write_in_pwd(option, args[i], args[0], e));
    if (!args[i + 2])
        return (cd_search_in_pwd(args + i, args[0], e));
    ft_error(e, "%s: too many arguments: %s", args[0], args[i + 3]);
    return (1);
}
