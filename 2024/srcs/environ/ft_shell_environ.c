/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_shell_environ.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/01/20 12:12:34 by gbourgeo          #+#    #+#             */
/*   Updated: 2014/03/27 19:21:14 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_defines.h"
#include "ft_shell_environ.h"
#include "ft_shell_history.h"
#include "ft_shell_log.h"
#include "ft_snprintf.h"
#include "libft.h"
#include <pwd.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/param.h>
#include <sys/ttycom.h>
#include <unistd.h>

static const char *ft_env_get_value(const char *keyname, const char **env);

const char *ft_shell_env_get_value(const char *keyname, t_env *environ)
{
    const char *ptr = ft_env_get_value(keyname, (const char **) environ->public);

    if (ptr == NULL)
    {
        ptr = ft_env_get_value(keyname, (const char **) environ->private);
    }
    return (ptr);
}

void ft_shell_env_init(t_env *environ, const char **env, const char *shellname)
{
    const char    *shlvl           = NULL;
    struct passwd *passwd          = NULL;
    struct winsize winsize         = { 0 };
    char           cwd[MAXPATHLEN] = { 0 };

    environ->bin_path = ft_strdup("/usr/gnu/bin:/usr/local/bin:/usr/bin:/bin");
    /* Initialisation de l'environement publique */
    environ->public   = ft_tabdup(env);
    environ->private = NULL;
    (void) getcwd(cwd, sizeof(cwd));
    ft_shell_env_add(environ, "PWD", cwd, UINT32(SHELL_ENV_ADD_PUBLIC) | UINT32(SHELL_ENV_ADD_PRIVATE));
    shlvl = ft_env_get_value("SHLVL", (const char **) environ->public);
    ft_snprintf(cwd, sizeof(cwd), "%d", ft_atoi(shlvl) + 1);
    ft_shell_env_add(environ, "SHLVL", cwd, SHELL_ENV_ADD_PUBLIC);
    ft_shell_env_add(environ, "SHELL", shellname, UINT32(SHELL_ENV_ADD_PUBLIC) | UINT32(SHELL_ENV_ADD_PRIVATE));
    /* Initialisation de l'environement privé  */
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &winsize) != -1)
    {
        ft_snprintf(cwd, sizeof(cwd), "%d", winsize.ws_col);
        ft_shell_env_add(environ, "COLUMNS", cwd, SHELL_ENV_ADD_PRIVATE);
        ft_snprintf(cwd, sizeof(cwd), "%d", winsize.ws_row);
        ft_shell_env_add(environ, "LINES", cwd, SHELL_ENV_ADD_PRIVATE);
    }
    passwd = getpwuid(getuid());
    if (passwd != NULL)
    {
        ft_shell_env_add(environ, "HOME", passwd->pw_dir, SHELL_ENV_ADD_PRIVATE);
        ft_shell_env_add(environ, "LOGNAME", passwd->pw_name, SHELL_ENV_ADD_PRIVATE);
    }
    ft_shell_env_add(environ, "TMPDIR", "/tmp", SHELL_ENV_ADD_PRIVATE);
    ft_shell_env_add(environ, "IFS", " \n\t", SHELL_ENV_ADD_PRIVATE);
    ft_shell_env_add(environ, "42SH_HISTORY_SIZE", SHELL_HISTORY_MAX_ELEMS, SHELL_ENV_ADD_PRIVATE);
    ft_shell_env_add(environ, "PS1", "%F{white}%p %F{yellow}%u %F{red}%w %F{white}> ", SHELL_ENV_ADD_PRIVATE);
}

static char  *ft_shell_env_interpret_escaped_char(const char *value);
static char **ft_shell_env_add_key(char **env, const char *keyname, const char *keyvalue);

void ft_shell_env_add(t_env *environ, const char *keyname, const char *keyvalue, uint32_t options)
{
    char  *value = (char *) keyvalue;

    if (TEST_BIT(options, SHELL_ENV_INTERPRETCHARACTER))
    {
        value = ft_shell_env_interpret_escaped_char(keyvalue);
    }
    if (TEST_BIT(options, SHELL_ENV_ADD_PUBLIC))
    {
        environ->public = ft_shell_env_add_key(environ->public, keyname, keyvalue);
    }
    if (TEST_BIT(options, SHELL_ENV_ADD_PRIVATE))
    {
        environ->private = ft_shell_env_add_key(environ->private, keyname, keyvalue);
    }
    if (TEST_BIT(options, SHELL_ENV_INTERPRETCHARACTER))
    {
        free(value);
    }
}
void ft_shell_env_free(t_env *environ)
{
    free((void *) environ->bin_path);
    ft_freetab(&environ->public);
    ft_freetab(&environ->private);
}

void ft_shell_env_remove(t_env *environ, const char *keyname)
{
    char **env = environ->public;
    size_t pos = 0;

    if (env == NULL || keyname == NULL || keyname[0] == '\0')
    {
        return;
    }
    while (env[pos] != NULL)
    {
        if (ft_strcmp(env[pos], keyname) == '=')
        {
            free(env[pos]);
            while (env[pos + 1] != NULL)
            {
                env[pos] = env[pos + 1];
                pos++;
            }
            env[pos] = NULL;
        }
        pos++;
    }
}

/**
 * @brief Récupère la valeur d'une variable d'environement.
 * @param str Nom de la variable d'environement
 * @param env Environement
 * @return La valeur de la variable d'environement si elle existe,
 * NULL autrement.
 */
static const char *ft_env_get_value(const char *keyname, const char **env)
{
    size_t pos = 0;

    if (env == NULL || keyname == NULL)
    {
        return (NULL);
    }
    while (env[pos] != NULL)
    {
        if (ft_strcmp(env[pos], keyname) == '=')
        {
            return (env[pos] + ft_strlen(keyname) + 1);
        }
        pos++;
    }
    return (NULL);
}

/**
 * Interprète les caractères d'échappement connus tels que '\\a', '\\b', '\\t'
 * '\\n', '\\v', '\\f', '\\r' et '\\e' en les remplaçant par les valeurs que
 * l'on peut retrouver dans la table ASCII (cf. man ascii).
 * @param[in] value Chaîne de caractères à traiter
 * @return Une nouvelle chaîne de caractères qui devra être free.
 */
static char *ft_shell_env_interpret_escaped_char(const char *value)
{
    char  *ivalue = ft_strdup(value);
    size_t iter   = 0;

    while (ivalue[iter] != '\0')
    {
        if (ivalue[iter] == '\\')
        {
            if (ivalue[iter + 1] == 'a')
            {
                ivalue[iter] = '\a';
            }
            else if (ivalue[iter + 1] == 'b')
            {
                ivalue[iter] = '\b';
            }
            else if (ivalue[iter + 1] == 't')
            {
                ivalue[iter] = '\t';
            }
            else if (ivalue[iter + 1] == 'n')
            {
                ivalue[iter] = '\n';
            }
            else if (ivalue[iter + 1] == 'v')
            {
                ivalue[iter] = '\v';
            }
            else if (ivalue[iter + 1] == 'f')
            {
                ivalue[iter] = '\f';
            }
            else if (ivalue[iter + 1] == 'r')
            {
                ivalue[iter] = '\r';
            }
            else if (ivalue[iter + 1] == 'e')
            {
                ivalue[iter] = '\033';
            }
            if (ivalue[iter] != '\\') // On a interprété le '\', il faut donc copier le reste de la chaine à la suite
            {
                ft_strcpy(ivalue + iter + 1, ivalue + iter + 2);
            }
        }
        iter++;
    }
    return (ivalue);
}

/**
 * @brief Fonction de recherche d'une variable d'environement.
 *
 * @param env Environement
 * @param key Clé
 * @return L'adresse de la variable d'environement, NULL si pas trouvée.
 */
static char **ft_shell_env_add_key(char **env, const char *keyname, const char *keyvalue)
{
    size_t iter = 0;

    if (keyname == NULL || keyname[0] == '\0')
    {
        ft_shell_log(SH_LOG_LEVEL_ERR, "Invalid environement variable name given.");
        return (env);
    }
    if (env != NULL)
    {
        while (env[iter] != NULL)
        {
            if (ft_strcmp(env[iter], keyname) == '=')
            {
                free(env[iter]);
                break;
            }
            iter++;
        }
    }
    if (env == NULL || env[iter] == NULL)
    {
        size_t len = ft_tablen((const char **) env) + 2;
        char **ptr = (char **) malloc(sizeof(char *) * len);

        if (ptr == NULL)
        {
            ft_shell_log(SH_LOG_LEVEL_ERR, "Memory error: Adding '%s' to environment failed.", keyname);
            return (env);
        }
        len = 0;
        if (env != NULL)
        {
            while (env[len] != NULL)
            {
                ptr[len] = env[len];
                len++;
            }
            free((void *) env);
        }
        ptr[len + 1] = NULL;
        env          = ptr;
    }
    env[iter] = (char *) malloc(ft_strlen(keyname) + ft_strlen(keyvalue) + 2);
    ft_strcpy(env[iter], keyname);
    ft_strcat(env[iter], "=");
    ft_strcat(env[iter], keyvalue);
    return (env);
}
