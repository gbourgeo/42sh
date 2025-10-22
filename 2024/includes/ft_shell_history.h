/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_shell_history.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/01/20 12:12:34 by gbourgeo          #+#    #+#             */
/*   Updated: 2014/03/27 19:21:14 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _FT_SHELL_HISTORY_H_
#define _FT_SHELL_HISTORY_H_

#include "ft_defines.h"
#include "ft_shell_command.h"
#include "ft_shell_environ.h"
#include <stddef.h>
#include <stdint.h>

#define SHELL_HISTORY_MAX_ELEMS "100"

typedef struct _align(32) s_history
{
    const char *filepath; /* Fichier de l'Historique de Commandes */
    long        length;   /* Longueur de l'Historique de Commandes */
    long        size;     /* Taille (max) de l'Historique de Commandes */
} t_hist;

/**
 * @brief Initialise le contexte de l'historique.
 * @param history Contexte de l'historique du Shell
 */
void ft_shell_history_init(t_hist *history);

/**
 * Initialise un contexte d'historique de commandes à partir d'un fichier.
 * @param command Pointeur sur la première commande
 * @param history Contexte de l'Historique du Shell
 * @param environ Environnement du Shell
 * @param progname Nom du programme
 */
void ft_shell_history_parse_file(t_cmd     **command,
                                 t_hist     *history,
                                 t_env      *environ,
                                 const char *progname);

/**
 * @brief Sauvegarde toutes les commandes dans le fichier d'historique du Shell
 * @param history Historique du Shell
 * @param command Commandes du Shell
 */
void ft_shell_history_save_to_file(const t_hist *history, const t_cmd *command);

/**
 * Sauvegarde une nouvelle commande au début de la liste de commandes.
 * @param history Historique de Commandes
 * @param command Structure d'une Commande
 */
void ft_shell_history_save_command(t_hist *history, t_cmd *command);

#endif /* _FT_SHELL_HISTORY_H_ */
