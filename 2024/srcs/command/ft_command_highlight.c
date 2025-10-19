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

#include "ft_defines.h"
#include "ft_shell_command.h"
#include <stddef.h>
#include <stdlib.h>

static t_higharea *ft_highlight_merge(t_higharea *harea);
static t_higharea *ft_highlight_sort_by_position(t_higharea *harea);
static t_higharea *ft_command_highlight_remove_area(t_cmd      *command,
                                                    t_higharea *harea);

t_higharea *ft_command_highlight_remove_all(t_higharea *harea)
{
    t_higharea *next = NULL;

    harea = ft_command_highlight_first(harea);
    while (harea != NULL)
    {
        next = harea->next;
        free(harea);
        harea = next;
    }
    return (NULL);
}

t_higharea *ft_command_highlight_first(const t_higharea *harea)
{
    while (harea != NULL)
    {
        if (harea->prev == NULL)
        {
            return ((t_higharea *) harea);
        }
        harea = harea->prev;
    }
    return ((t_higharea *) harea);
}

t_higharea *ft_command_highlight_sort_area(t_higharea *harea)
{
    /* Rewind de la liste */
    harea = ft_command_highlight_first(harea);
    /* Réorganisation des zones */
    harea = ft_highlight_sort_by_position(harea);
    /* Merge des zones qui se chevauchent */
    harea = ft_highlight_merge(harea);
    return (harea);
}

void ft_command_highlight_move_areas(t_cmd *command, e_highop operation, size_t value)
{
    t_higharea *harea = ft_command_highlight_first(command->harea);
    size_t      pos   = command->pos;
    int         hmode = TEST_BIT(command->option, COMMAND_HIGHLIGHT_MODE);

    while (harea != NULL)
    {
        switch (operation)
        {                                         /* **************************************************** */
            case SHELL_HIGHLIGHTED_AREA_ADD_CHAR: /* Ajout d'un caractère                                 */
            {                                     /*                                                      */
                if (pos < harea->tail)            /* La position ne se trouve pas dans une zone de texte  */
                {                                 /* Hareas:   | - |   *   | - - - - - - - |   | - - |    */
                    harea->tail += value;         /* Result:   | - |     *   | - - - - - - - |   | - - |  */
                    harea->head += value;         /*           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0  */
                } /*                                                      */
                else if (pos == harea->tail)  /* La position se trouve au début d'une zone de texte   */
                {                             /* Highlight Mode : OFF                                 */
                    if (hmode == 0)           /* Hareas:   | - |       * - - - - - - - |   | - - |    */
                    {                         /* Result:   | - |           * - - - - - - - |   | - -  */
                        harea->tail += value; /*           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0  */
                    } /* Highlight Mode : ON                                  */
                    harea->head += value; /* Hareas:   | - |       * - - - - - - - |   | - - |    */
                } /* Result:   | - |       | - * - - - - - - - |   | - -  */
                /*           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0  */
                else if (pos < harea->head) /* La position se trouve dans une zone de texte         */
                {                           /* Hareas:   | - |       | - - * - - - - |   | - - |    */
                    harea->head += value;   /* Result:   | - |       | - - - * - - - - |   | - - |  */
                } /*           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0  */
                else if (pos == harea->head)  /* La position se trouve à la fin d'une zone de texte   */
                {                             /* Highlight Mode : OFF                                 */
                    if (hmode != 0)           /* Hareas:   | - |       | - - - - - - - *   | - - |    */
                    {                         /* Result:   | - |       | - - - - - - - | *   | - - |  */
                        harea->head += value; /*           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0  */
                    } /* Highlight Mode : ON                                  */
                } /* Hareas:   | - |       | - - - - - - - *   | - - |    */
                break; /* Result:   | - |       | - - - - - - - - *   | - - |  */
            } /*           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0  */
                /* **************************************************** */
            case SHELL_HIGHLIGHTED_AREA_REMOVE_CHAR_LEFT: /* Suppression d'un caractère à gauche            */
            {                                             /*                                                      */
                if (pos <= harea->tail)                   /* La position ne se trouve pas dans une zone de texte  */
                {                                         /* Hareas:   | - |   *   | - - - - - - - |   | - - |    */
                    harea->tail -= value;                 /* Result:   | - | *   | - - - - - - - |     | - - |    */
                    harea->head -= value;                 /*           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0  */
                } /* La position se trouve au début d'une zone de texte   */
                /* Hareas:   | - |       * - - - - - - - |   | - - |    */
                /* Result:   | - |     * - - - - - - - |     | - - |    */
                /*           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0  */
                else if (pos <= harea->head) /* La position se trouve dans une zone de texte         */
                {                            /* Hareas:   | - |       | - - * - - - - |   | - - |    */
                    harea->head -= value;    /* Result:   | - |       | - * - - - - |     | - - |    */
                } /*           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0  */
                /* La position se trouve à la fin d'une zone de texte   */
                /* Hareas:   | - |       | - - - - - - - *   | - - |    */
                /* Result:   | - |       | - - - - - - *     | - - |    */
                /*           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0  */
                else if (pos > harea->head)         /* La position se trouve après une zone de texte        */
                {                                   /*                                                      */
                    if (pos - value <= harea->tail) /* et la value se trouve avant cette zone de texte      */
                    {                               /* Hareas:   | - |       | - - - - - - - | *   | - - |  */
                        harea = ft_command_highlight_remove_area(command, harea);
                        /* Result:   | - |       *                     | - - |  */
                        continue; /*           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0  */
                    } /*                                                      */
                    if (pos - value < harea->head) /* et la value se trouve dans cette zone de texte       */
                    {                              /* Hareas:   | - |       | - - - - - - - | *   | - - |  */
                        harea->head = pos - value; /* Result:   | - |       | - - - *             | - - |  */
                    } /*           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0  */
                } /*                                                      */
                break; /*                                                      */
            } /* **************************************************** */
            case SHELL_HIGHLIGHTED_AREA_REMOVE_CHAR_RIGHT: /* Suppression d'un caractère à droite           */
            {                                              /*                                                      */
                if (pos < harea->tail)                     /* La position ne se trouve pas dans une zone de texte  */
                {                                          /*                                                      */
                    if (pos + value >= harea->head)        /* et la value se trouve après cette zone de texte      */
                    {                                      /* on supprime la zone et on passe à la suivante...     */
                        harea = ft_command_highlight_remove_area(command, harea);
                        continue;                          /* Hareas:   | - |   *   | - - - - - - - |   | - - |    */
                    } /* Result:   | - |   *                       | - - |    */
                    /*           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0  */
                    if (pos + value >= harea->tail) /* et la value se trouve dans cette zone de texte       */
                    {                               /* Hareas:   | - |   *   | - - - - - - - |   | - - |    */
                        harea->tail = pos;          /* Result:   | - |   * - - - - |             | - - |    */
                    } /*           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0  */
                    else                      /*                                                      */
                    {                         /* Hareas:   | - |   *   | - - - - - - - |   | - - |    */
                        harea->tail -= value; /* Result:   | - |   * | - - - - - - - |     | - - |    */
                    } /*           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0  */
                    harea->head -= value; /*                                                      */
                } /*                                                      */
                else if (pos == harea->tail)                                          /* La position se trouve au début d'une zone de texte   */
                {                                                                     /*                                                      */
                    if (pos + value >= harea->head)                                   /* et la value se trouve après cette zone de texte      */
                    {                                                                 /* Highlight Mode : OFF                                 */
                        if (hmode == 0)                                               /* Hareas:   | - |       * - - - - - - - |   | - - |    */
                        {                                                             /* Result:   | - |       *                   | - - |    */
                            harea = ft_command_highlight_remove_area(command, harea); /* on supprime la zone et         */
                            continue;                                                 /* on passe à la zone suivante... */
                        } /*                                                      */
                        harea->head = harea->tail; /* Highlight Mode : ON                                  */
                    } /* on conserve la zone de texte surlignée...            */
                    else                      /*                                                      */
                    {                         /* Hareas:   | - |       * - - - - - - - |   | - - |    */
                        harea->head -= value; /* Result:   | - |       * - - - |           | - - |    */
                    } /*           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0  */
                } /*                                                      */
                else if (pos < harea->head)         /* La position se trouve dans une zone de texte         */
                {                                   /* Hareas:   | - |       | - - - * - - - |   | - - |    */
                    if (pos + value >= harea->head) /* et la value se trouve après cette zone de texte      */
                    {                               /* Hareas:   | - |       | - - - * - - - |   | - - |    */
                        harea->head = pos;          /* Result:   | - |       | - - - *           | - - |    */
                    } /*           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0  */
                    else                      /* et la value se trouve dans cette zone de texte       */
                    {                         /* Hareas:   | - |       | - - - * - - - |   | - - |    */
                        harea->head -= value; /* Result:   | - |       | - - - * - |       | - - |    */
                    } /*           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0  */
                } /*                                                      */
                /* La position se trouve à la fin d'une zone de texte   */
                break; /* -> rien à faire.                                     */
            } /* **************************************************** */
            case SHELL_HIGHLIGHTED_AREA_MOVE_LEFT: /* Déplacement de la position vers la gauche            */
            {                                      /*                                                      */
                if (hmode == 0)                    /* Highlight Mode : OFF                                 */
                {                                  /*                                                      */
                    break;                         /* -> rien à faire                                      */
                } /*                                                      */
                if (pos == harea->tail)   /* La position se trouve au début d'une zone de texte   */
                {                         /* Hareas:   | - |       * - - - - - - - |   | - - |    */
                    harea->tail -= value; /* Result:   | - |   * - - - - - - - - - |   | - - |    */
                } /*           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0  */
                else if (pos < harea->head)        /* La position se trouve dans une zone de texte         */
                {                                  /*                                                      */
                    if (pos - value < harea->tail) /* et la value est avant la tail                        */
                    {                              /* Hareas:   | - |       | - - - * - - - |   | - - |    */
                        harea->tail = pos - value; /* Result:   | - |   * - - - - - - - - - |   | - - |    */
                    } /*           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0  */
                } /*                                                      */
                else if (pos == harea->head)        /* La position se trouve à la fin d'une zone de texte   */
                {                                   /*                                                      */
                    if (pos - value <= harea->tail) /* et la value est avant la tail                        */
                    {                               /* Hareas:   | - |       | - - - - - - - *   | - - |    */
                        harea->head = harea->tail;  /* Result:   | - |   * - |                   | - - |    */
                        harea->tail = pos - value;  /*           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0  */
                    } /*                                                      */
                    else                      /* sinon                                                */
                    {                         /* Hareas:   | - |       | - - - - - - - *   | - - |    */
                        harea->head -= value; /* Result:   | - |       | - - - *           | - - |    */
                    } /*           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0  */
                } /*                                                      */
                else if (pos > harea->head)                                       /* La position se trouve après une zone de texte        */
                {                                                                 /* Hareas:   | - |       | - - - - - - - *   | - - |    */
                    if (pos - value <= harea->tail)                               /* Result:               | - - - - - - - *   | - - |    */
                    {                                                             /*           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0  */
                        harea = ft_command_highlight_remove_area(command, harea); /* on supprime la zone et     */
                        continue;                                                 /* on passe à la suivante...  */
                    } /*                                                      */
                } /* Sinon rien à faire.                                  */
                break; /*                                                      */
            } /* **************************************************** */
            case SHELL_HIGHLIGHTED_AREA_MOVE_RIGHT: /* Déplacement de la position vers la droite            */
            {                                       /*                                                      */
                if (hmode == 0)                     /* Highlight Mode : OFF                                 */
                {                                   /*                                                      */
                    break;                          /* -> Rien à faire                                      */
                } /*                                                      */
                if (pos < harea->tail)                                            /* La position se trouve avant d'une zone de texte      */
                {                                                                 /* Hareas:   | - |       * - - - - - - - |   | - - |    */
                    if (pos + value >= harea->head)                               /* Result:   | - |                       | - - - - *    */
                    {                                                             /*           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0  */
                        harea = ft_command_highlight_remove_area(command, harea); /* on supprime la zone et     */
                        continue;                                                 /* on passe à la suivante...  */
                    } /*                                                      */
                } /*                                                      */
                if (pos < harea->head)             /* La position se trouve au début d'une zone de texte   */
                {                                  /* et la value est supérieure à la head                 */
                    if (pos + value > harea->head) /* Hareas:   | - |       * - - - - - - - |   | - - |    */
                    {                              /* Result:   | - |                       | - - - - *    */
                        harea->tail = harea->head; /*           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0  */
                        harea->head = pos + value; /* Sinon                                                */
                    } /* Hareas:   | - |       * - - - - - - - |   | - - |    */
                    else                      /* Result:   | - |               * - - - |   | - - |    */
                    {                         /*           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0  */
                        harea->tail += value; /* La position se trouve dans une zone de texte         */
                    } /* et la value est supérieure à la head                 */
                } /* Hareas:   | - |       | - - - * - - - |   | - - |    */
                /* Result:   | - |                       | - - - - *    */
                /*           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0  */
                /* Sinon                                                */
                /* Hareas:   | - |       * - - - - - - - |   | - - |    */
                /* Result:   | - |               * - - - |   | - - |    */
                /*           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0  */
                else if (pos == harea->head) /* La position se trouve à la fin d'une zone de texte   */
                {                            /* Hareas:   | - |       | - - - - *         | - - |    */
                    harea->head += value;    /* Hareas:   | - |       | - - - - - - - *   | - - |    */
                } /*           0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0  */
                break; /*                                                      */
            }
        }
        harea = harea->next;
    }
}

/**
 * @brief Fonction de suppression d'une zone de texte surlignée d'une commande.
 * @param[in] command Structure d'une commande
 * @param[in] harea Structure d'une zone de texte surlignée
 * @return La zone de texte surlignée suivante.
 */
static t_higharea *ft_command_highlight_remove_area(t_cmd      *command,
                                                    t_higharea *harea)
{
    t_higharea *next = NULL;

    if (harea != NULL)
    {
        next = harea->next;
        if (harea->prev != NULL)
        {
            harea->prev->next = next;
        }
        else
        {
            command->harea = next;
        }
        if (next != NULL)
        {
            next->prev = harea->prev;
        }
        free(harea);
    }
    return (next);
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
    return (ft_command_highlight_first(harea));
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
    return (ft_command_highlight_first(harea));
}
