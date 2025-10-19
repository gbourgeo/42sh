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

#include "ft_defines.h"
#include "ft_printf.h"
#include "ft_shell.h"
#include "ft_shell_builtins.h"
#include "ft_shell_environ.h"
#include "ft_shell_log.h"
#include "libft.h"
#include <stdint.h>
#include <stdlib.h>

/**
 * Recherche les options du builtin et affiche l'usage si demandé.
 * @param[in] args          Liste des arguments
 * @param[out] interpret    Pointeur sur les options supportés
 * @return La position du premier argument qui n'est pas une option,
 * 0 si l'affichage de l'usage est demandé,
 * -1 sur une mauvaise option ou si pas d'argument donné.
 */
static long ft_setenv_option(const char **args, uint32_t *interpret)
{
    long iter = 1;

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
            *interpret = SHELL_ENV_INTERPRETCHARACTER;
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
    char    *ptr   = NULL;
    uint32_t inter = 0;
    long  iter  = ft_setenv_option(args, &inter);

    if (iter <= 0)
    {
        return (int) (-iter);
    }
    while (args[iter])
    {
        ptr = ft_strchr(args[iter], '=');
        if (ptr != NULL)
        {
            *ptr = '\0';
            ft_shell_env_add(&shell->environ, args[iter], ptr + 1, UINT32(SHELL_ENV_ADD_PUBLIC) | inter);
            *ptr = '=';
        }
        else if (args[iter + 1] != NULL)
        {
            ft_shell_env_add(&shell->environ, args[iter], args[iter + 1], UINT32(SHELL_ENV_ADD_PUBLIC) | inter);
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
