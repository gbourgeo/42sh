/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_setenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/01/20 23:26:39 by gbourgeo          #+#    #+#             */
/*   Updated: 2017/02/23 05:37:38 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "ft_shell.h"
#include "ft_shell_builtins.h"
#include "ft_shell_log.h"
#include "libft.h"
#include <stddef.h>
#include <stdlib.h>

/**
 * Recherche une variable d'environnement dans la table.
 * @param[in] key       Nom de la variable d'environnement (sans le '=')
 * @param[in] env       Table d'environnement
 * @return La position dans la table de la variable d'environnement,
 * -1 si elle n'a pas été trouvée.
 */
static ssize_t search_in_env(const char *key, char * const *env)
{
    ssize_t iter = 0;

    if (key == NULL || env == NULL)
    {
        return (-1);
    }
    while (env[iter] != NULL)
    {
        if (ft_strcmp(env[iter], key) == '=')
        {
            return (iter);
        }
        iter++;
    }
    return (-1);
}

/**
 * Créé une nouvelle variable d'environnement sous la forme: key=value
 * @param[in] key       Nom de la variable d'environnement
 * @param[in] value     Valeur de la variable d'environnement
 * @return Une nouvelle variable d'environnement
 */
static char *new_env_var(const char *key, const char *value)
{
    char *new_var = NULL;

    new_var = (char *) malloc(ft_strlen(key) + ft_strlen(value) + 2);
    ft_strcpy(new_var, key);
    ft_strcat(new_var, "=");
    ft_strcat(new_var, value);
    return (new_var);
}

/**
 * Duplique la table en entrée en y ajoutant la nouvelle variable d'environnement
 * 'key=value' à la fin de la nouvelle table. Les variables d'environnement à l'intérieur
 * de la table passée en argument ne sont pas dupliquées, seule la table la regroupant l'est.
 * @param[in] key       Nom de la variable d'environnement
 * @param[in] value     Valeur de la variable d'environnement
 * @param[in] env_old   Table d'environnement à modifier
 * @return Une nouvelle table d'environnement avec la nouvelle variable.
 */
static char **new_env(const char *key, const char *value, char **env_old)
{
    size_t len     = ft_tablen((const char **) env_old) + 2;
    char **env_new = (char **) malloc(sizeof(char *) * len);

    if (env_new == NULL)
    {
        return (NULL);
    }
    len = 0;
    if (env_old != NULL)
    {
        while (env_old[len])
        {
            env_new[len] = env_old[len];
            len++;
        }
        free((void *) env_old);
    }
    env_new[len]     = new_env_var(key, value);
    env_new[len + 1] = NULL;
    return (env_new);
}

/**
 * Interprète les caractères d'échappement connus tels que '\\a', '\\b', '\\t'
 * '\\n', '\\v', '\\f', '\\r' et '\\e' en les remplaçant par les valeurs que
 * l'on peut retrouver dans la table ASCII (cf. man ascii).
 * @param[in] value Chaîne de caractères à traiter
 * @return Une nouvelle chaîne de caractères qui devra être free.
 */
static char *ft_interpret_escape_char(const char *value)
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
 * @brief Fonction interne qui ajoute ou modifie une variable d'environnement.
 * @param[in] key       Clé
 * @param[in] value     Valeur
 * @param[in] env       Environnement
 * @param[in] interpret Bit d'interprétation des caractère d'échappement ('\n', '\t', etc.)
 * @return Un nouvel environnement
 */
char **ft_modify_env(const char *key, const char *value, char **env, char interpret)
{
    char   *ivalue = (char *) value;
    ssize_t pos    = 0;

    if (interpret != 0)
    {
        ivalue = ft_interpret_escape_char(value);
    }
    pos = search_in_env(key, env);
    if (pos >= 0)
    {
        free(env[pos]);
        env[pos] = new_env_var(key, ivalue);
    }
    else
    {
        env = new_env(key, ivalue, env);
    }
    if (interpret != 0)
    {
        free(ivalue);
    }
    return (env);
}

/**
 * Recherche les options du builtin et affiche l'usage si demandé.
 * @param[in] args          Liste des arguments
 * @param[out] interpret    Pointeur sur les options supportés
 * @return La position du premier argument qui n'est pas une option,
 * 0 si l'affichage de l'usage est demandé,
 * -1 sur une mauvaise option ou si pas d'argument donné.
 */
static ssize_t ft_setenv_option(const char **args, char *interpret)
{
    ssize_t iter = 1;

    while (args[iter] && args[iter][0] == '-')
    {
        if (ft_strcmp(args[iter], "-h") == 0 || ft_strcmp(args[iter], "--help") == 0)
        {
            ft_printf("Usage: %s [OPTION]... [NAME=VALUE]... [[NAME] [VALUE]...]\n\n"
                      "Options:\n"
                      "  -x    Interpret escape characters from VALUE,"
                      " like '\\n', '\\t', '\\e', etc.\n\n"
                      "Single arguments must have the '=' sign to form the combination \"NAME=VALUE\",\n"
                      "on the other hand, arguments can be separated with a space, like \"NAME VALUE\".\n",
                      args[0]);
            return (0);
        }
        if (ft_strcmp(args[iter], "-x") == 0)
        {
            *interpret = 1;
        }
        else if (args[iter][0] == '-')
        {
            ft_shell_log(SH_LOG_LEVEL_WARN, "%s: bad option '%s'", args[0], args[iter]);
            return (-1);
        }
        iter++;
    }
    if (args[iter] == NULL)
    {
        ft_shell_log(SH_LOG_LEVEL_WARN, "%s: bad assignment", args[0]);
        return (-1);
    }
    return (iter);
}

/**
 * @brief Builtin "setenv" qui ajoute ou modifie une variable d'environnement.
 * Si pas d'argument, la fonction retourne une erreur.
 * Les arguments peuvent être de la forme "VAR=value" ou "VAR VALUE"
 * @param[in] args      Tableau d'argument
 * @param[in] shell_ctx Contexte du shell
 * @return 0 pour succès
 */
int ft_setenv(const char **args, t_shell *shell)
{
    char   *ptr       = NULL;
    char    interpret = 0;
    ssize_t iter      = ft_setenv_option(args, &interpret);

    if (iter <= 0)
    {
        return (int) (-iter);
    }
    while (args[iter])
    {
        ptr = ft_strchr(args[iter], '=');
        if (ptr != NULL)
        {
            *ptr              = '\0';
            shell->global_env = ft_modify_env(args[iter], ptr + 1, shell->global_env, interpret);
            *ptr              = '=';
        }
        else if (args[iter + 1] != NULL)
        {
            shell->global_env = ft_modify_env(args[iter], args[iter + 1], shell->global_env, interpret);
            iter++;
        }
        else
        {
            ft_shell_log(SH_LOG_LEVEL_WARN, "%s: missing value: '%s'", args[0], args[iter]);
            return (1);
        }
        iter++;
    }
    return (0);
}
