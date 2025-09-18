/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_highlight_yank.c                                :+:      :+:    :+:   */
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
#include "ft_shell_termkeys.h"
#include "libft.h"
#include <stdint.h>
#include <stdlib.h>

void ft_highlight_yank(t_shell *shell)
{
    t_higharea *harea = NULL;
    uint8_t    *yank  = NULL;
    size_t      len   = 0;

    if (harea == NULL)
    {
        return;
    }
    /* Réorganisation des zones */
    shell->command->harea = ft_highlight_sort_area(shell->command->harea);
    harea                 = shell->command->harea;
    /* Suppression de l'ancienne copie */
    ft_freestr((char **) &shell->yank);
    /* Calcul de la nouvelle taille de la copie */
    while (harea != NULL)
    {
        len += (harea->head - harea->tail) + (len != 0);
        harea = harea->next;
    }
    /* Création de la copie */
    yank    = (uint8_t *) malloc(len);
    yank[0] = '\0';
    harea   = shell->command->harea;
    len     = 0;
    while (harea != NULL)
    {
        if (yank[0] != '\0')
        {
            yank[len] = ' ';
            len++;
        }
        ft_memcpy(yank + len, shell->command->buffer + harea->tail, harea->head - harea->tail);
        len += harea->head - harea->tail;
        harea = harea->next;
    }
    yank[len]   = '\0';
    shell->yank = yank;
    /* Désactive le mode de surlignage */
    REMOVE_BIT(shell->command->option, COMMAND_HIGHLIGHT_MODE);
    /* Suppression des zones surlignées */
    shell->command->harea = ft_highlight_remove_all(shell->command->harea);
    /* Réécriture de la ligne de commande sans surlignage */
    ft_shell_command_print(shell->command,
                           &shell->terminal,
                           UINT32(COMMAND_PRINT_FROM_START));
}
