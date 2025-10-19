/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_shell_environ.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2013/12/28 04:49:32 by gbourgeo          #+#    #+#             */
/*   Updated: 2018/04/06 17:34:07 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _FT_SHELL_ENVIRON_H
#define _FT_SHELL_ENVIRON_H

#include "ft_defines.h"
#include <stdint.h>

typedef struct _align(32) s_shell_environnement
{
    const char  *bin_path; /* Chemins absolus des binaires */
    char       **public;   /* Variables d'environnement publiques */
    char       **private;  /* Variables d'environnement privées */
} t_env;

enum e_shell_environ_modify_options
{
    SHELL_ENV_INTERPRETCHARACTER = 0x1,
    SHELL_ENV_ADD_PUBLIC      = 0x2,
    SHELL_ENV_ADD_PRIVATE     = 0x4,
};

/**
 * @brief Récupère la valeur d'une variable d'environnement.
 * Cette fonction recherche dans l'environnement globale et privé du Shell.
 * @param keyname Nom de la varialble d'environnement
 * @param shell Structure du Shell
 * @return La valeur de la variable d'environnement si elle existe,
 *         NULL si elle n'existe pas.
 */
const char *ft_shell_env_get_value(const char *keyname, t_env *environ);

/**
 * @brief Fonction d'initialisation de l'environnement du Shell, à savoir:
 *
 * - Duplication des variables d'environnement du processus appelant (parent),
 *
 * - Ajout/Modification des variables d'environement PWD, SHLVL, SHELL, COLUMNS,
 *   LINES, HOME, LOGNAME, TMPDIR, IFS, 42SH_HISTORY_SIZE et PS1.
 *
 * @param[in] environ       Environnement du Shell
 * @param[in] env           Environnement parent
 * @param[in] shellname     Nom du shell
 */
void ft_shell_env_init(t_env *environ, const char **env, const char *shellname);

/**
 * @brief Fonction d'ajout/modification d'une variable d'Environement du Shell.
 * La fonction recherche d'abord la variable dans un(plusieurs) environement du Shell,
 * si la variable n'existe pas elle sera ajoutée.
 * Options disponibles:
 *
 * - SHELL_ENV_INTERPRETCHARACTER, interprète les caractères d'échappement de la valeur.
 *
 * - SHELL_ENV_ADD_PUBLIC, modifie l'environement global du Shell.
 *
 * - SHELL_ENV_ADD_PRIVATE, modifie l'environement privé du Shell.
 *
 * @param[in] env   Environnement du Shell
 * @param[in] key   Clé
 * @param[in] value Nouvelle valeur de clé
 * @param[in] options Options de modification
 */
void ft_shell_env_add(t_env *environ, const char *key, const char *value, uint32_t options);

/**
 * @brief Supprime les éléments de l'Environement du Shell.
 * @param environ Environement du Shell
 */
void ft_shell_env_free(t_env *environ);

/**
 * @brief Fonction de modification d'une variable d'Environement du Shell.
 * Si la variable n'existe pas elle ne sera pas ajoutée.
 * Options disponibles:
 *
 * - SHELL_ENV_INTERPRETCHARACTER, interprète les caractères d'échappement de la valeur.
 *
 * - SHELL_ENV_ADD_PUBLIC, modifie l'environement global du Shell.
 *
 * - SHELL_ENV_ADD_PRIVATE, modifie l'environement privé du Shell.
 *
 * @param[in] env   Environnement du Shell
 * @param[in] key   Clé
 * @param[in] value Nouvelle valeur de clé
 * @param[in] options Options de modification
 */
void ft_shell_env_modify(t_env *environ, const char *key, const char *value, uint32_t options);

/**
 * @brief Supprime une variable de l'environement global.
 * @param environ Environement du Shell
 * @param keyname Nom de la variable
 */
void ft_shell_env_remove(t_env *environ, const char *keyname);

#endif /* _FT_SHELL_ENVIRON_H */
