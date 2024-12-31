/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_highlight.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/03/13 16:03:26 by gbourgeo          #+#    #+#             */
/*   Updated: 2018/04/06 18:39:13 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_HIGHLIGHT_H
#define FT_HIGHLIGHT_H

#include <stddef.h>

typedef struct s_highlighted_text
{
    size_t                     head; /* Dernier caractère surligné de la commande */
    size_t                     tail; /* Premier caractère surligné de la commande */
    struct s_highlighted_text *next; /* Texte surligné suivant */
    struct s_highlighted_text *prev; /* Texte surligné précédent */
} t_htext;

typedef struct s_highlight
{
    char     on;    /* Indique si le mode de surlignage est actif ou non */
    t_htext *texts; /* Liste simplement chainé de textes surlignés */
    char    *yank;  /* Sauvegarde du texte surligné */
} t_hlight;

/**
 * @brief Initialise la structure highlight.
 * @param hlight Structure highlight
 */
void     ft_init_shell_highlight(t_hlight *hlight);

/**
 * @brief Fonction de récupération d'une zone de texte surlignée.
 * @param[in] pos Position actuelle du curseur
 * @param[in] htexts Liste des zones surlignées
 * @return Si la position actuelle du curseur se trouve sur une zone surlignée
 * celle-ci est retournée, NULL sinon.
 */
t_htext *ft_highlight_get_area(size_t pos, t_htext *htexts);

/**
 * @brief Fonction de suppression de toutes les zones surlignées dans la liste.
 * Elle supprime également le surlignage des zones.
 * @param[in] shell_ctx Structure d'environnement du shell
 */
void     ft_highlight_remove_all(t_htext **htexts);

/**
 * @brief Fonction de suppression de zone de surlignage.
 * @param[in] htext Zone de surlignage à supprimer
 * @return La zone précédente si elle existe, la suivante sinon
 */
t_htext *ft_highlight_remove_area(t_htext *htext);

/**
 * @brief Fonction de réarrangement des zones surlignées.
 * La tete (head) et la queue (tail) sont réorganisées de telle sorte que la
 * tete soit toujours supérieure à la queue.
 * Les zones sont ensuite triées par ordre croissant, la première zone
 * surlignée du texte sera placée au début de la liste.
 * @param[in] text Liste des zones surlignées
 */
t_htext *ft_highlight_sort_area(t_htext *text);

#endif /* FT_highlight_H */
