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

#include "ft_shell_environ.h"
#include "ft_defines.h"
#include "ft_dynamic_table.h"
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

/************************************/
/* FONCTIONS PRIVEES                */
/************************************/

static const char *internal_ft_shell_env_get_value(const char *keyname, const t_dyntab *environ);
static void        internal_ft_shell_env_init_dyntab(t_dyntab *environ, const char **env);
static void        internal_ft_shell_env_elem_init(void *elem);
static void        internal_ft_shell_env_elem_dup(void *elem, void *todup);
static void        internal_ft_shell_env_elem_free(void *elem);
static char       *internal_ft_shell_env_interpret_escaped_char(const char *value);
static void        internal_ft_shell_env_add_s_key(t_dyntab *environ, const char *keyname, const char *keyvalue);

/************************************/
/* FONCTIONS PUBLIQUES              */
/************************************/

void ft_shell_env_add_envvar(t_dyntab *environ, const char *envvar)
{
    size_t      iter = 0;
    size_t      len  = 0;
    size_t      size = 0;
    const char *ptr  = NULL;
    t_envelem  *elem = NULL;

    ptr = ft_strchr(envvar, '=');
    if (ptr == NULL)
    {
        return;
    }
    size = (size_t) (ptr - envvar);
    /* Recherche dans la table */
    len  = ft_dynamic_table_get_len(environ);
    while (iter < len)
    {
        elem = (t_envelem *) ft_dynamic_table_get_elem_pos(environ, iter);
        if (ft_strncmp(elem->envvar, envvar, size) == '=')
        {
            free(elem->envvar);
            elem->envvar = ft_strdup(envvar);
            return;
        }
        iter++;
    }
    elem         = (t_envelem *) ft_dynamic_table_new_elem(environ);
    elem->envvar = ft_strdup(envvar);
}

void ft_shell_env_add_l(t_env *environ, const char *keyname, long keyvalue, uint32_t options)
{
    char value[256] = { 0 };

    ft_snprintf(value, sizeof(value), "%ld", keyvalue);
    if (_test_bit(options, SHELL_ENV_ADD_PUBLIC))
    {
        internal_ft_shell_env_add_s_key(&environ->public, keyname, value);
    }
    if (_test_bit(options, SHELL_ENV_ADD_PRIVATE))
    {
        internal_ft_shell_env_add_s_key(&environ->private, keyname, value);
    }
}

void ft_shell_env_add_s(t_env *environ, const char *keyname, const char *keyvalue, uint32_t options)
{
    char *value = (char *) keyvalue;

    if (_test_bit(options, SHELL_ENV_INTERPRETCHARACTER))
    {
        value = internal_ft_shell_env_interpret_escaped_char(keyvalue);
    }
    if (_test_bit(options, SHELL_ENV_ADD_PUBLIC))
    {
        internal_ft_shell_env_add_s_key(&environ->public, keyname, keyvalue);
    }
    if (_test_bit(options, SHELL_ENV_ADD_PRIVATE))
    {
        internal_ft_shell_env_add_s_key(&environ->private, keyname, keyvalue);
    }
    if (_test_bit(options, SHELL_ENV_INTERPRETCHARACTER))
    {
        free(value);
    }
}

void ft_shell_env_free(t_env *environ)
{
    free((void *) environ->bin_path);
    ft_dynamic_table_free(&environ->public);
    ft_dynamic_table_free(&environ->private);
}

t_envelem *ft_shell_env_get_elem(const t_dyntab *environ, const char *keyname)
{
    t_envelem *elem = NULL;
    size_t     pos  = 0;
    size_t     len  = 0;

    if (keyname == NULL || keyname[0] == '\0')
    {
        return (NULL);
    }
    len = ft_dynamic_table_get_len(environ);
    while (pos < len)
    {
        elem = ft_dynamic_table_get_elem_pos(environ, pos);
        if (ft_strcmp(elem->envvar, keyname) == '=')
        {
            return (elem);
        }
        pos++;
    }
    return (NULL);
}

const char *ft_shell_env_get_value(const char *keyname, const t_env *environ)
{
    t_envelem *elem = ft_shell_env_get_elem(&environ->public, keyname);

    if (elem == NULL)
    {
        elem = ft_shell_env_get_elem(&environ->private, keyname);
    }
    if (elem == NULL)
    {
        return (NULL);
    }
    return (elem->envvar + ft_strlen(keyname) + 1);
}

void ft_shell_env_init(t_env *environ, const char **env, const char *shellname)
{
    const char    *shlvl           = NULL;
    struct passwd *passwd          = NULL;
    struct winsize winsize         = { 0 };
    char           cwd[MAXPATHLEN] = { 0 };

    environ->bin_path = ft_strdup("/usr/gnu/bin:/usr/local/bin:/usr/bin:/bin");
    /* Initialisation de l'environement publique */
    internal_ft_shell_env_init_dyntab(&environ->public, env);
    internal_ft_shell_env_init_dyntab(&environ->private, NULL);
    (void) getcwd(cwd, sizeof(cwd));
    ft_shell_env_add_s(environ,
                       "PWD",
                       cwd,
                       _uint32(SHELL_ENV_ADD_PUBLIC)
                           | _uint32(SHELL_ENV_ADD_PRIVATE));
    shlvl = internal_ft_shell_env_get_value("SHLVL",
                                            (const t_dyntab *) &environ->public);
    ft_shell_env_add_l(environ,
                       "SHLVL",
                       ft_atoi(shlvl) + 1,
                       SHELL_ENV_ADD_PUBLIC);
    ft_shell_env_add_s(environ,
                       "SHELL",
                       shellname,
                       _uint32(SHELL_ENV_ADD_PUBLIC)
                            | _uint32(SHELL_ENV_ADD_PRIVATE));
    /* Initialisation de l'environement privé  */
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &winsize) != -1)
    {
        ft_shell_env_add_l(environ,
                           "COLUMNS",
                           (long) winsize.ws_col,
                           SHELL_ENV_ADD_PRIVATE);
        ft_shell_env_add_l(environ,
                           "LINES",
                           (long) winsize.ws_row,
                           SHELL_ENV_ADD_PRIVATE);
    }
    passwd = getpwuid(getuid());
    if (passwd != NULL)
    {
        ft_shell_env_add_s(environ,
                           "HOME",
                           passwd->pw_dir,
                           SHELL_ENV_ADD_PRIVATE);
        ft_shell_env_add_s(environ,
                           "LOGNAME",
                           passwd->pw_name,
                           SHELL_ENV_ADD_PRIVATE);
    }
    ft_shell_env_add_s(environ,
                       "TMPDIR",
                       "/tmp",
                       SHELL_ENV_ADD_PRIVATE);
    ft_shell_env_add_s(environ,
                       "IFS",
                       " \t\n",
                       _uint32(SHELL_ENV_ADD_PRIVATE)
                            | _uint32(SHELL_ENV_INTERPRETCHARACTER));
    ft_shell_env_add_s(environ,
                       "42SH_HISTORY_SIZE",
                       SHELL_HISTORY_MAX_ELEMS,
                       SHELL_ENV_ADD_PRIVATE);
    ft_shell_env_add_s(environ,
                       "PS1",
                       "%F{white}%p %F{yellow}%u %F{red}%w %F{white}> ",
                       SHELL_ENV_ADD_PRIVATE);
    ft_shell_env_add_l(environ,
                       "PPID",
                       (long) getppid(),
                       SHELL_ENV_ADD_PRIVATE);
}

void ft_shell_env_remove(t_env *environ, const char *keyname)
{
    t_envelem *elem = NULL;
    size_t     pos  = 0;
    size_t     len  = 0;

    if (keyname == NULL || keyname[0] == '\0')
    {
        return;
    }
    len = ft_dynamic_table_get_len(&environ->public);
    while (pos < len)
    {
        elem = ft_dynamic_table_get_elem_pos(&environ->public, pos);
        if (ft_strcmp(elem->envvar, keyname) == '=')
        {
            ft_dynamic_table_remove_elem_pos(&environ->public, pos);
            return;
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
static const char *internal_ft_shell_env_get_value(const char *keyname, const t_dyntab *environ)
{
    t_envelem *elem = NULL;
    size_t     pos  = 0;
    size_t     len  = 0;

    if (keyname == NULL || keyname[0] == '\0')
    {
        return (NULL);
    }
    len = ft_dynamic_table_get_len(environ);
    while (pos < len)
    {
        elem = ft_dynamic_table_get_elem_pos(environ, pos);
        if (ft_strcmp(elem->envvar, keyname) == '=')
        {
            return (elem->envvar + ft_strlen(keyname) + 1);
        }
        pos++;
    }
    return (NULL);
}

/**
 * @brief Initialise une Table Dynamique.
 * @param environ Table dynamique d'Environnement
 * @param env     Environnement à dupliquer
 */
static void internal_ft_shell_env_init_dyntab(t_dyntab *environ, const char **env)
{
    t_envelem *envelem = NULL;
    size_t     iter    = 0;

    ft_dynamic_table_init(environ,
                          sizeof(t_envelem),
                          SHELL_ENVIRON_ELEMS,
                          internal_ft_shell_env_elem_init,
                          internal_ft_shell_env_elem_dup,
                          internal_ft_shell_env_elem_free);
    if (env == NULL)
    {
        return;
    }
    while (env[iter] != NULL)
    {
        envelem         = (t_envelem *) ft_dynamic_table_new_elem(environ);
        envelem->envvar = ft_strdup(env[iter]);
        iter++;
    }
}

/**
 * @brief Initialise un élément de la Table Dynamique
 * @param elem Elément de la Table Dynamique
 */
static void internal_ft_shell_env_elem_init(void *elem)
{
    t_envelem *envelem = elem;

    envelem->envvar = NULL;
}

/**
 * @brief Duplique un élément d'une table.
 * @param elem Elément de la Table Dynamique
 * @param todup Elément à dupliquer
 */
static void internal_ft_shell_env_elem_dup(void *elem, void *todup)
{
    t_envelem *envelem   = elem;
    t_envelem *todupelem = todup;

    envelem->envvar = ft_strdup(todupelem->envvar);
}

/**
 * @brief Désalloue un élément de la Table Dynamique
 * @param elem Elément de la Table Dynamique
 */
static void internal_ft_shell_env_elem_free(void *elem)
{
    t_envelem *envelem = elem;

    free(envelem->envvar);
    envelem->envvar = NULL;
}

/**
 * Interprète les caractères d'échappement connus tels que '\\a', '\\b', '\\t'
 * '\\n', '\\v', '\\f', '\\r' et '\\e' en les remplaçant par les valeurs que
 * l'on peut retrouver dans la table ASCII (cf. man ascii).
 * @param[in] value Chaîne de caractères à traiter
 * @return Une nouvelle chaîne de caractères qui devra être free.
 */
static char *internal_ft_shell_env_interpret_escaped_char(const char *value)
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
 * @param environ Environement
 * @param keyname Clé
 * @param keyvalue Valeur
 */
static void internal_ft_shell_env_add_s_key(t_dyntab *environ, const char *keyname, const char *keyvalue)
{
    size_t     iter = 0;
    size_t     len  = 0;
    t_envelem *elem = NULL;

    if (keyname == NULL || keyname[0] == '\0')
    {
        ft_shell_log(SH_LOG_LEVEL_ERR, "Invalid environment variable name given.");
        return;
    }
    /* Recherche dans la table */
    len = ft_dynamic_table_get_len(environ);
    while (iter < len)
    {
        elem = (t_envelem *) ft_dynamic_table_get_elem_pos(environ, iter);
        if (ft_strcmp(elem->envvar, keyname) == '=')
        {
            free(elem->envvar);
            elem->envvar = (char *) malloc(ft_strlen(keyname) + ft_strlen(keyvalue) + 2);
            ft_strcpy(elem->envvar, keyname);
            ft_strcat(elem->envvar, "=");
            ft_strcat(elem->envvar, keyvalue);
            return;
        }
        iter++;
    }
    elem         = (t_envelem *) ft_dynamic_table_new_elem(environ);
    elem->envvar = (char *) malloc(ft_strlen(keyname) + ft_strlen(keyvalue) + 2);
    ft_strcpy(elem->envvar, keyname);
    ft_strcat(elem->envvar, "=");
    ft_strcat(elem->envvar, keyvalue);
}
