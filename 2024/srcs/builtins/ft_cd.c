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

#include "ft_builtins.h"
#include "ft_log.h"
#include "libft.h"

#include <sys/stat.h>
#include <unistd.h>

static int ft_cd_chdir_error(char *pwd, const char *dirname, const char *cmdname)
{
    struct stat buffer;
    int         ret;

    if (pwd)
    {
        ret = stat(pwd, &buffer);
        if (ret == -1)
            if (lstat(pwd, &buffer) != -1)
                ft_log(SH_LOG_LEVEL_WARN, "%s: too many levels of symbolic links: %s", cmdname, dirname);
            else
                ft_log(SH_LOG_LEVEL_WARN, "%s: no such file or directory: %s", cmdname, dirname);
        else if (!S_ISDIR(buffer.st_mode))
            ft_log(SH_LOG_LEVEL_WARN, "%s: not a directory: %s", cmdname, dirname);
        else
            ft_log(SH_LOG_LEVEL_WARN, "%s: permission denied: %s", cmdname, dirname);
        free(pwd);
    }
    return (1);
}

static int cd_write_in_pwd(cd_e option, const char *dirname, const char *cmdname, t_shell *shell)
{
    char *pwd;
    char *tmp;

    pwd = cd_check(dirname, cmdname, shell);
    if (chdir(pwd) != -1)
    {
        tmp = pwd;
        if (option & CD_RESOLVE_BEFORE)
        {
            pwd = ft_getcwd(tmp, shell);
            free(tmp);
        }
        cd_change_pwds(pwd, shell);
        free(pwd);
        return (0);
    }
    return (ft_cd_chdir_error(pwd, dirname, cmdname));
}

static char *cd_change_in_pwd(const char **dirs, const char *found, const char *pwd)
{
    char *tmp;
    char *tmp_2;

    tmp   = ft_strndup(pwd, ft_strlen(pwd) - ft_strlen(found));
    tmp_2 = ft_strjoin(tmp, dirs[0]);
    free(tmp);
    tmp = ft_strjoin(tmp_2, found + ft_strlen(dirs[1]));
    free(tmp_2);
    return (tmp);
}

static int cd_search_in_pwd(const char **dirs, const char *arg_name, t_shell *shell)
{
    char *pwd;
    char *tmp;

    pwd = (char *) ft_getenv("PWD", shell);
    if ((tmp = ft_strstr(pwd, dirs[0])) == NULL)
    {
        ft_log(SH_LOG_LEVEL_WARN, "%s: string not in pwd: %s", arg_name, dirs[0]);
        return (1);
    }
    pwd = cd_change_in_pwd(dirs, tmp, pwd);
    if (chdir(pwd) != -1)
    {
        cd_change_pwds(pwd, shell);
        ft_putendl(pwd);
        free(pwd);
        return (0);
    }
    return (ft_cd_chdir_error(pwd, dirs[0], arg_name));
}

static int ft_cd_invalid_option(char option, const char *cmd_name)
{
    ft_log(SH_LOG_LEVEL_WARN, "%s: invalid option: -%c", cmd_name, option);
    ft_log(SH_LOG_LEVEL_WARN, "usage: %s [-L|-P] [dir]", cmd_name);
    return (1);
}

int ft_cd(const char **args, t_shell *shell)
{
    cd_e option;
    int  i;
    int  j;

    option = CD_RESOLVE_UNDEFINED;
    i      = 1;
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
                return (ft_cd_invalid_option(args[i][j], args[0]));
            j++;
        }
        i++;
    }
    if (!args[i] || !args[i + 1])
        return (cd_write_in_pwd(option, args[i], args[0], shell));
    if (!args[i + 2])
        return (cd_search_in_pwd(args + i, args[0], shell));
    ft_log(SH_LOG_LEVEL_WARN, "%s: too many arguments: %s", args[0], args[i + 3]);
    return (1);
}
