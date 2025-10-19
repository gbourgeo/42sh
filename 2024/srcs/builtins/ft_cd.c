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

#include "ft_defines.h"
#include "ft_shell.h"
#include "ft_shell_builtins.h"
#include "ft_shell_environ.h"
#include "ft_shell_log.h"
#include "libft.h"
#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static int ft_cd_chdir_error(char *pwd, const char *dirname, const char *cmdname)
{
    struct stat buffer;
    int         ret = 0;

    if (pwd != NULL)
    {
        ret = stat(pwd, &buffer);
        if (ret == -1)
        {
            if (lstat(pwd, &buffer) != -1)
            {
                ft_shell_log(SH_LOG_LEVEL_WARN, "%s: too many levels of symbolic links: %s", cmdname, dirname);
            }
            else
            {
                ft_shell_log(SH_LOG_LEVEL_WARN, "%s: no such file or directory: %s", cmdname, dirname);
            }
        }
        else if (!S_ISDIR(buffer.st_mode))
        {
            ft_shell_log(SH_LOG_LEVEL_WARN, "%s: not a directory: %s", cmdname, dirname);
        }
        else
        {
            ft_shell_log(SH_LOG_LEVEL_WARN, "%s: permission denied: %s", cmdname, dirname);
        }
        free(pwd);
    }
    return (1);
}

static char *cd_change_in_pwd(const char **dirs, const char *found, const char *pwd)
{
    char *tmp   = NULL;
    char *tmp_2 = NULL;

    tmp   = ft_strndup(pwd, ft_strlen(pwd) - ft_strlen(found));
    tmp_2 = ft_strjoin(tmp, dirs[0]);
    free(tmp);
    tmp = ft_strjoin(tmp_2, found + ft_strlen(dirs[1]));
    free(tmp_2);
    return (tmp);
}

static int cd_search_in_pwd(const char **dirs, const char *arg_name, t_shell *shell)
{
    char *pwd = NULL;
    char *tmp = NULL;

    pwd = (char *) ft_shell_env_get_value("PWD", &shell->environ);
    tmp = ft_strstr(pwd, dirs[0]);
    if (tmp == NULL)
    {
        ft_shell_log(SH_LOG_LEVEL_WARN, "%s: string not in pwd: %s", arg_name, dirs[0]);
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

static size_t internal_cd_parse_args(const char **args,
                                     cd_e *option);
static char *internal_cd_get_path_from_env(const char *varname,
                                           const char *toappend,
                                           const char *cmdname,
                                           t_env      *environ);
static int internal_cd_change_directory(char *path,
                                        cd_e option,
                                        const char *cmdname,
                                        t_shell *shell);
static void internal_cd_change_directory_env(const char *path,
                                             t_env *environ);

int ft_cd(const char **args, t_shell *shell)
{
    cd_e   option = CD_RESOLVE_UNDEFINED;
    size_t iter   = internal_cd_parse_args(args, &option);
    char  *path   = NULL;

    /* No args */
    if (!args[iter])
    {
        path = internal_cd_get_path_from_env("HOME",
                                             NULL,
                                             args[0],
                                             &shell->environ);
        path = ft_shell_resolve_path(path);
    }
    /* One arg */
    if (!args[iter + 1])
    {
        if (args[iter][0] == '~')
        {
            path = internal_cd_get_path_from_env("HOME",
                                                 args[iter] + 1,
                                                 args[0],
                                                 &shell->environ);
        }
        else if (ft_strcmp(args[iter], "-") == 0)
        {
            path = internal_cd_get_path_from_env("OLDPWD",
                                                 NULL,
                                                 args[0],
                                                 &shell->environ);
        }
        else if (args[iter][0] == '/')
        {
            path = ft_strdup(args[iter]);
        }
        else
        {
            path = internal_cd_get_path_from_env("PWD",
                                                 args[iter],
                                                 args[0],
                                                 &shell->environ);
        }
        return (internal_cd_change_directory(path, option, args[0], shell));
    }
    /* Two args */
    if (!args[iter + 2])
    {
        return (cd_search_in_pwd(args + iter, args[0], shell));
    }
    /* More than two arguments */
    ft_shell_log(SH_LOG_LEVEL_WARN, "%s: too many arguments: %s", args[0], args[iter + 3]);
    return (1);
}

/**
 * @brief Parse les arguments du builtin.
 * @param args Liste d'arguments
 * @param option Résultat d'option
 @return La position du premier argument qui n'est pas une option.
 */
static size_t internal_cd_parse_args(const char **args, cd_e *option)
{
    size_t iter = 1;

    while (args[iter] && args[iter][0] == '-')
    {
        size_t jter = 1;
        char   chr  = args[iter][jter];

        if (chr == '\0') /* cd - */
        {
            return (iter);
        }
        while (chr != '\0')
        {
            if (ft_strcmp(args[iter], "--") == 0)
            {
                return (iter + 1);
            }
            if (chr == 'P')
            {
                *option = CD_RESOLVE_BEFORE;
            }
            else if (chr == 'L')
            {
                *option = CD_RESOLVE_AFTER;
            }
            else
            {
                ft_shell_log(SH_LOG_LEVEL_WARN, "%s: invalid option: -%c", args[0], option);
                ft_shell_log(SH_LOG_LEVEL_WARN, "usage: %s [-L|-P] [dir]", args[0]);
                return (iter);
            }
            jter++;
        }
        iter++;
    }
    return (iter);
}

/**
 * @brief Fonction de récupération de la variable d'environement HOME et de
 * concaténation avec la variable passée en argument.
 * @param varname Nom de la variable d'environement à récupérer
 * @param toappend Complément du chemin à ajouter
 * @param cmdname Nom du builtin
 * @param environ Environement du Shell
 * @return Le chemin du HOME concaténé avec l'argument.
 * Le chemin devra être libéré avec free().
 */
static char *internal_cd_get_path_from_env(const char *varname,
                                           const char *toappend,
                                           const char *cmdname,
                                           t_env      *environ)
{
    const char *varvalue = ft_shell_env_get_value(varname, environ);
    char       *path     = NULL;
    size_t      len      = 0;

    if (varvalue == NULL)
    {
        ft_shell_log(SH_LOG_LEVEL_WARN, "%s: %s not defined", cmdname, varname);
        return (NULL);
    }
    if (toappend != NULL && *toappend == '/')
    {
        toappend++;
    }
    len = ft_strlen(varvalue);
    len += (varvalue[len - 1] != '/') ? 1 : 0;
    len += (toappend != NULL) ? ft_strlen(toappend) : 0;
    path = (char *) malloc(len + 1);
    path = ft_strcpy(path, varvalue);
    path = (varvalue[len - 1] != '/') ? ft_strcat(path, "/") : path;
    path = (toappend != NULL) ? ft_strcat(path, toappend) : path;
    return (path);
}

/**
 * @brief Change de répertoire par l'appel à chdir().
 * @param path      Nouveau chemin
 * @param option    Option
 * @param cmdname   Nom du builtin
 * @param shell
 * @return 0 si OK, 1 autrement.
 */
static int internal_cd_change_directory(char       *path,
                                        cd_e        option,
                                        const char *cmdname,
                                        t_shell    *shell)
{
    path = ft_shell_resolve_path(path);
    if (chdir(path) == -1)
    {
        ft_shell_log(SH_LOG_LEVEL_ERR, "%s: %s", cmdname, strerror(errno));
        errno = 0;
        // return (ft_cd_chdir_error(path, dirname, cmdname)); // TODO(gbo): @gbo: supp ?
        return (1);
    }
    if (TEST_BIT(option, CD_RESOLVE_BEFORE))
    {
        char *save = path;

        path = getcwd(NULL, 0);
        if (path == NULL)
        {
            ft_shell_log(SH_LOG_LEVEL_WARN, "getcwd: %s", strerror(errno));
            errno = 0;
            path = save;
        }
        else
        {
            free(save);
        }
    }
    internal_cd_change_directory_env(path, &shell->environ);
    free(path);
    return (0);
}

static void internal_cd_change_directory_env(const char *path, t_env *environ)
{
    const char *pwd = ft_shell_env_get_value("PWD", environ);
    ft_shell_env_add(environ,
                     "OLDPWD",
                     pwd,
                     UINT32(SHELL_ENV_ADD_PUBLIC));

    ft_shell_env_add(environ,
                     "PWD",
                     path,
                     UINT32(SHELL_ENV_ADD_PUBLIC));
}
