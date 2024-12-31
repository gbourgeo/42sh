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

#include "ft_builtins.h"
#include "ft_log.h"
#include "ft_printf.h"
#include "libft.h"

/**
 * Recherche une variable d'environnement dans la table.
 * @param[in] key       Nom de la variable d'environnement (sans le '=')
 * @param[in] env       Table d'environnement
 * @return La position dans la table de la variable d'environnement,
 * -1 si elle n'a pas été trouvée.
 */
static int search_in_env(const char *key, char * const *env)
{
    int i;

    i = 0;
    if (key == NULL || env == NULL)
        return (-1);
    while (env[i] != NULL)
    {
        if (ft_strcmp(env[i], key) == '=')
            return (i);
        i++;
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
    char *new_var;

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
    char **env_new;
    int    i;

    i       = ft_tablen(env_old) + 2;
    env_new = (char **) malloc(sizeof(char *) * i);
    if (env_new == NULL)
        return (NULL);
    i = 0;
    if (env_old)
    {
        while (env_old[i])
        {
            env_new[i] = env_old[i];
            i++;
        }
        free(env_old);
    }
    env_new[i]     = new_env_var(key, value);
    env_new[i + 1] = NULL;
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
    size_t i      = 0;

    while (ivalue[i] != '\0')
    {
        if (ivalue[i] == '\\')
        {
            if (ivalue[i + 1] == 'a')
                ivalue[i] = '\a';
            else if (ivalue[i + 1] == 'b')
                ivalue[i] = '\b';
            else if (ivalue[i + 1] == 't')
                ivalue[i] = '\t';
            else if (ivalue[i + 1] == 'n')
                ivalue[i] = '\n';
            else if (ivalue[i + 1] == 'v')
                ivalue[i] = '\v';
            else if (ivalue[i + 1] == 'f')
                ivalue[i] = '\f';
            else if (ivalue[i + 1] == 'r')
                ivalue[i] = '\r';
            else if (ivalue[i + 1] == 'e')
                ivalue[i] = '\e';
            if (ivalue[i] != '\\') // On a interprété le '\', il faut donc copier le reste de la chaine à la suite
                ft_strcpy(ivalue + i + 1, ivalue + i + 2);
        }
        i++;
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
    char *ivalue;
    int   i;

    ivalue = (char *) value;
    if (interpret != 0)
        ivalue = ft_interpret_escape_char(value);
    if ((i = search_in_env(key, env)) >= 0)
    {
        free(env[i]);
        env[i] = new_env_var(key, ivalue);
    }
    else
        env = new_env(key, ivalue, env);
    if (interpret != 0)
        free(ivalue);
    return (env);
}

/**
 * Recherche les options du builtin et affiche l'usage si demandé.
 * @param[in] args          Liste des arguments
 * @param[out] interpret    Pointeur sur les options supportés
 * @param[in] shell         Contexte du shell
 * @return La position du premier argument qui n'est pas une option,
 * 0 si l'affichage de l'usage est demandé,
 * -1 sur une mauvaise option ou si pas d'argument donné.
 */
static int ft_setenv_option(const char **args, int *interpret)
{
    size_t i = 1;

    while (args[i] && args[i][0] == '-')
    {
        if (ft_strcmp(args[i], "-h") == 0 || ft_strcmp(args[i], "--help") == 0)
        {
            ft_printf("Usage: %s [OPTION]... [NAME=VALUE]... [[NAME] [VALUE]...]\n"
                      "\n"
                      "Options:\n"
                      "  -x    Interpret escape characters from VALUE,"
                      " like '\\n', '\\t', '\\e', etc.\n"
                      "\n"
                      "Single arguments must have the '=' sign to form the combination \"NAME=VALUE\",\n"
                      "on the other hand, arguments can be separated with a space, like \"NAME VALUE\".\n",
                      args[0]);
            return (0);
        }
        else if (ft_strcmp(args[i], "-x") == 0)
            *interpret = 1;
        else if (args[i][0] == '-')
        {
            ft_log(SH_LOG_LEVEL_WARN, "%s: bad option '%s'", args[0], args[i]);
            return (-1);
        }
        i++;
    }
    if (args[i] == NULL)
    {
        ft_log(SH_LOG_LEVEL_WARN, "%s: bad assignment", args[0]);
        return (-1);
    }
    return (i);
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
    char *ptr       = NULL;
    int   interpret = 0;
    int   i;

    i = ft_setenv_option(args, &interpret);
    if (i <= 0)
        return (-i);
    while (args[i])
    {
        ptr = ft_strchr(args[i], '=');
        if (ptr != NULL)
        {
            *ptr              = '\0';
            shell->global_env = ft_modify_env(args[i], ptr + 1, shell->global_env, interpret);
            *ptr              = '=';
        }
        else if (args[i + 1] != NULL)
        {
            shell->global_env = ft_modify_env(args[i], args[i + 1], shell->global_env, interpret);
            i++;
        }
        else
        {
            ft_log(SH_LOG_LEVEL_WARN, "%s: missing value: '%s'", args[0], args[i]);
            return (1);
        }
        i++;
    }
    return (0);
}
