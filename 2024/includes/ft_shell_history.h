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
#include <stddef.h>
#include <stdint.h>

#define SHELL_HISTORY_MAX_ELEMS "1024"

typedef struct _align(16) s_history
{
    const char *filepath;
    long        max_size;
} t_hist;

/**
 * @brief Initialise le contexte de l'historique.
 * @param history Contexte de l'historique du Shell
 */
void ft_shell_history_init(t_hist *history);

/**
 * Initialise un contexte d'historique de commandes à partir d'un fichier.
 * @param command Pointeur sur la première commande
 * @param history Contexte de l'historique du Shell
 * @param progname Nom du programme
 * @param environ Environnement du Shell
 * @return Dernière commande de la liste de commande
 */
size_t ft_shell_history_parse_file(t_cmd       **command,
                                   t_hist      *history,
                                   const char  *progname,
                                   const char **environ);

/**
 * @brief Sauveagrde l'historique dans le fichier d'hitosique du Shell
 * @param history Contexte de l'historique du Shell
 * @param historic Liste de commandes
 */
void ft_shell_history_save_to_file(t_hist *history, t_cmd *historic);

#endif /* _FT_SHELL_HISTORY_H_ */
