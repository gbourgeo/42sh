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
#include "ft_shell_terminal.h"
#include "libft.h"
#include <stdint.h>
#include <stdlib.h>

/**
 * @brief Calcule la longueur de toutes les zones de texte surligné.
 * @param harea Liste de zone de texte surligné
 * @return La longueur totale des zones de texte surligné.
 */
static size_t ft_highlight_len(t_higharea *harea)
{
    size_t len = 0;

    /* Calcul de la nouvelle taille de la copie */
    while (harea != NULL)
    {
        len += (harea->head - harea->tail) + (len != 0);
        harea = harea->next;
    }
    return (len);
}

/**
 * @brief Copie des zones de texte surligné dans le buffer de destination.
 * @param dst Buffer de destination
 * @param src Buffer source
 * @param harea Zones de texte surligné
 */
static void ft_highlight_copy(uint8_t *dst, uint8_t *src, t_higharea *harea)
{
    size_t pos = 0;

    while (harea != NULL)
    {
        if (pos != 0)
        {
            dst[pos] = ' ';
            pos++;
        }
        ft_memcpy(dst + pos, src + harea->tail, harea->head - harea->tail);
        pos += harea->head - harea->tail;
        harea = harea->next;
    }
    dst[pos] = '\0';
}

void ft_highlight_yank(t_shell *shell)
{
    size_t length = 0;
    /* Highlight mode : actif */
    if (TEST_BIT(shell->command->option, COMMAND_HIGHLIGHT_MODE))
    {
        REMOVE_BIT(shell->command->option, COMMAND_HIGHLIGHT_MODE);
        ft_term_clear_modes(&shell->terminal);
        shell->command->harea = ft_command_highlight_sort_area(shell->command->harea);
    }
    shell->command->harea = ft_command_highlight_first(shell->command->harea);
    length                = ft_highlight_len(shell->command->harea);
    if (shell->command->harea == NULL || length == 0)
    {
        return;
    }
    /* Suppression de l'ancienne copie */
    ft_freestr((char **) &shell->yank);
    /* Création de la copie */
    shell->yank = (uint8_t *) malloc(length);
    /* Copie */
    ft_highlight_copy(shell->yank, shell->command->buffer, shell->command->harea);
    /* Suppression des zones surlignées */
    shell->command->harea = ft_command_highlight_remove_all(shell->command->harea);
    /* Réécriture de la ligne de commande sans surlignage */
    ft_shell_command_print(shell->command, &shell->terminal, COMMAND_PRINT_FROM_START);
}
