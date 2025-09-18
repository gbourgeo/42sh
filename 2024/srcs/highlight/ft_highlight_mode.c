/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_highlight_mode.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/03/13 16:03:26 by gbourgeo          #+#    #+#             */
/*   Updated: 2018/04/06 18:39:13 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_defines.h"
#include "ft_shell.h"
#include "ft_shell_command.h"
#include "ft_shell_terminal.h"
#include <stdlib.h>

/**
 * @brief Fonction de création d'une nouvelle zone de surlignage et insertion de
 * celle-ci avant celle passée en paramètre.
 * @param[in] htexts Liste de zone de texte surlignée
 * @param[in] pos Position actuelle du curseur
 * @return Une nouvelle zone surlignée
 */
static t_higharea *ft_highlight_new_area(t_higharea *htexts, size_t pos)
{
    t_higharea *ptr = (t_higharea *) malloc(sizeof(*ptr));

    ptr->head = pos;
    ptr->tail = pos;

    if (htexts == NULL)
    {
        ptr->prev = NULL;
        ptr->next = NULL;
    }
    else
    {
        if (htexts->prev != NULL)
        {
            htexts->prev->next = ptr;
        }
        ptr->prev    = htexts->prev;
        ptr->next    = htexts;
        htexts->prev = ptr;
    }
    return (ptr);
}

/**
 * @brief Retourne une zone de texte surlignées contenant la position actuelle
 * dans le buffer de commande.
 * @param[in] htexts Liste des zones surlignées
 * @param[in] pos Position actuelle du curseur
 * @return Zone surlignée de la position ou une nouvelle zone de surlignage.
 */
static t_higharea *ft_highlight_get_area(t_higharea *htexts, size_t pos)
{
    t_higharea *first = ft_highlight_first(htexts);

    if (first != NULL)
    {
        htexts = first;
        /* Parcours */
        while (htexts != NULL)
        {
            if (pos < htexts->tail)
            {
                /* Création d'une nouvelle zone */
                return (ft_highlight_new_area(htexts, pos));
            }
            if (pos <= htexts->head)
            {
                return (htexts);
            }
            htexts = htexts->next;
        }
    }
    /* Création d'une nouvelle zone */
    return (ft_highlight_new_area(first, pos));
}

void ft_highlight_mode(t_shell *shell)
{
    if (TEST_BIT(shell->command->option, COMMAND_HIGHLIGHT_MODE))   /* Highlight mode : actif */
    {
        REMOVE_BIT(shell->command->option, COMMAND_HIGHLIGHT_MODE); /* Désactive l'option */
        ft_term_clear_modes(&shell->terminal);                      /* Désactive tous les modes actifs */
        shell->command->harea = ft_highlight_sort_area(shell->command->harea);
    }
    else                                                            /* Highlight mode : inactif */
    {
        ASSIGN_BIT(shell->command->option, COMMAND_HIGHLIGHT_MODE); /* Active l'option */
        ft_term_highlight_mode_on(&shell->terminal);                /* Active le mode surlignage */
        shell->command->harea = ft_highlight_get_area(shell->command->harea, shell->command->pos);
    }
}
