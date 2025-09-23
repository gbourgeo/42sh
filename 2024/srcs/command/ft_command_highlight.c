/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_command_highlight.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/02/12 22:32:55 by gbourgeo          #+#    #+#             */
/*   Updated: 2014/03/27 18:50:55 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_shell_command.h"
#include <stddef.h>
#include <stdlib.h>

static t_higharea *ft_highlight_merge(t_higharea *harea);
static t_higharea *ft_highlight_sort_by_position(t_higharea *harea);

t_higharea *ft_highlight_remove_all(t_higharea *harea)
{
    t_higharea *next = NULL;

    harea = ft_highlight_first(harea);
    while (harea != NULL)
    {
        next = harea->next;
        free(harea);
        harea = next;
    }
    return (NULL);
}

t_higharea *ft_highlight_first(const t_higharea *harea)
{
    if (harea != NULL)
    {
        while (harea->prev != NULL)
        {
            harea = harea->prev;
        }
    }
    return ((t_higharea *) harea);
}

t_higharea *ft_highlight_sort_area(t_higharea *harea)
{
    /* Rewind de la liste */
    harea = ft_highlight_first(harea);
    /* Réorganisation des zones */
    harea = ft_highlight_sort_by_position(harea);
    /* Merge des zones qui se chevauchent */
    harea = ft_highlight_merge(harea);
    return (harea);
}

/**
 * @brief Merge les zones qui se chevauchent.
 * @param harea Liste des zones de texte surlignées
 * @return Le premier élément de la liste.
 */
static t_higharea *ft_highlight_merge(t_higharea *harea)
{
    if (harea == NULL)
    {
        return (NULL);
    }
    /* Parcours de la liste */
    while (harea->next != NULL)
    {
        if ((harea->head >= harea->next->tail && harea->head < harea->next->head)
            || (harea->next->head <= harea->head))
        {
            t_higharea *next = harea->next;

            harea->next = harea->next->next;
            if (harea->next != NULL)
            {
                harea->next->prev = harea;
            }
            if (harea->head < next->head)
            {
                harea->head = next->head;
            }
            /* Désallocation de la zone de surlignage */
            free(next);
            continue;
        }
        harea = harea->next;
    }
    return (ft_highlight_first(harea));
}

/**
 * @brief Réorganise les zones de texte surlignées par leurs positions dans le buffer.
 * @param harea Liste de zone de texte surlignée
 * @return Le premier élément de la liste.
 */
static t_higharea *ft_highlight_sort_by_position(t_higharea *harea)
{
    if (harea == NULL)
    {
        return (NULL);
    }
    /* Parcours de la liste */
    while (harea->next != NULL)
    {
        /* Swap des zones */
        if (harea->tail > harea->next->tail)
        {
            t_higharea *prev     = harea->prev;
            t_higharea *next     = harea->next;
            t_higharea *far_next = harea->next->next;

            /* Swapping B et C :                */
            /*              *    *              */
            /* NULL <- A <- B <- C <- D         */
            /*         A -> B -> C -> D -> NULL */
            /* ******************************** */
            if (prev != NULL)      /* NULL <- A <- B <- C <- D         */
            {                      /*         A    B -> C -> D -> NULL */
                prev->next = next; /*           ------>                */
            } /*                                  */
            harea->prev = next;         /*            ----->                */
                                        /* NULL <- A  ^ B <- C <- D         */
                                        /*         A    B -> C -> D -> NULL */
                                        /*           ------>                */
                                        /* ******************************** */
            harea->next = far_next;     /*            ----->                */
                                        /* NULL <- A  ^ B <- C <- D         */
                                        /*         A    B    C -> D -> NULL */
                                        /*          \     --/--->           */
                                        /*           -------                */
                                        /* ******************************** */
            next->prev  = prev;         /*                <------           */
                                        /* NULL <- A <- C <- B    D         */
                                        /*         A -> C    B -> D -> NULL */
                                        /*                ------>           */
                                        /* ******************************** */
            next->next  = harea;        /*                <------           */
                                        /* NULL <- A <- C <- B    D         */
                                        /*         A -> C -> B -> D -> NULL */
                                        /* ******************************** */
            if (far_next != NULL)       /*                                  */
            {                           /* NULL <- A <- C <- B <- D         */
                far_next->prev = harea; /*         A -> C -> B -> D -> NULL */
                                        /*                                  */
            } /* ******************************** */
            if (next->prev == NULL) /* 1er élément */
            {
                continue;
            }
            harea = prev; /* On reprend depuis l'élément précédent */
            continue;
        }
        harea = harea->next;
    }
    return (ft_highlight_first(harea));
}
