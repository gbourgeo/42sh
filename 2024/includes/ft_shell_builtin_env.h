/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_shell_builtin_env.h                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/01/20 13:02:31 by gbourgeo          #+#    #+#             */
/*   Updated: 2017/02/23 05:37:15 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _FT_SHELL_BUILTIN_ENV_H_
#define _FT_SHELL_BUILTIN_ENV_H_

#include "ft_defines.h"
#include "ft_dynamic_table.h"
#include "ft_shell.h"
#include <stddef.h>
#include <stdint.h>

typedef enum env_options
{
    ENV_ERROR            = -1,
    ENV_NO_OPTION        = 0x0000,
    ENV_NO_ENV           = 0x0001,
    ENV_PRINT_NO_NEWLINE = 0x0002,
    ENV_VERBOSE          = 0x0004,
    ENV_PRINT_HELP       = 0x0008,
} env_e;

typedef struct _align(128) s_builtin_env
{
    const char  *builtin_name;
    t_dyntab     cpy;
    t_dyntab     save;
    const char  *change_dir;
    const char  *working_dir;
    const char **unset_env;
    const char **set_env;
    unsigned int option;
} t_builtin_env;

typedef struct _align(16) builtin_env_options
{
    char value;
    int  (*handler)(t_builtin_env *, const char *, size_t, const char **);
} t_builtin_env_opt;

/**
 * @brief Parse les arguments passés au builtin "env".
 * @param shell Environnement du shell
 * @param env   Environnement du builtin "env"
 * @param args  Liste des arguments
 * @param pos   Pointeur sur la position de l'argument qui n'est pas une option du builtin
 * @return -1 si erreur, 0 autrement.
 */
int ft_env_option_parser(t_shell *shell, t_builtin_env *env, const char **args, size_t *pos);

/**
 * @brief Ajout une variable à la table de variables passées en argument.
 * @param table Table de variables
 * @param var   Variable à ajouter
 * @return La nouvelle table de variables, NULL si erreur.
 */
const char **ft_env_add_var_to_table(char **table, const char *var);

#endif /* _FT_SHELL_BUILTIN_ENV_H_ */
