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

#include "ft_highlight.h"
#include "ft_shell.h"
#include "ft_termios.h"
#include <stdlib.h>
#include <unistd.h>

/**
 * @brief Fonction de création d'une nouvelle zone de surlignage.
 * @param[in] pos Position actuelle du curseur
 * @param[in] htexts Liste des zones surlignées
 * @return Zone surlignée de la position ou une nouvelle zone de surlignage
 */
static t_htext *ft_highlight_new_area(size_t pos, t_htext *htexts)
{
    t_htext *ptr = ft_highlight_get_area(pos, htexts);

    if (ptr == NULL)
    {
        ptr       = (t_htext *) malloc(sizeof(*ptr));
        ptr->head = pos;
        ptr->tail = pos;
        ptr->next = NULL;
        ptr->prev = NULL;
        if (htexts && htexts->prev)
        {
            htexts->prev->next = ptr;
            ptr->prev          = htexts->prev;
        }
        if (htexts && htexts->next)
        {
            htexts->next->prev = ptr;
            ptr->next          = htexts;
        }
    }
    return (ptr);
}

void ft_highlight_mode(t_shell *shell)
{
    t_htext *text = NULL;

    shell->highlighted.on = !shell->highlighted.on;
    if (shell->highlighted.on == 0)
    {
        text = shell->highlighted.texts;
        if (text->head >= text->tail)
        {
            ft_term_clear_modes(&shell->terminal); /* Désactive tous les modes actifs */
        }
        if (shell->command.buffer[shell->command.pos] != '\0')
        {
            ft_term_insert_mode_on(&shell->terminal);   /* Active le mode insertion */
            ft_term_remove_char(&shell->terminal);      /* Efface 1 caractère sous la position du curseur */
            write(STDOUT_FILENO, &shell->command.buffer[shell->command.pos], 1);
            ft_term_move_cursor_left(&shell->terminal); /* Bouge le curseur à gauche d'1 colonne */
            ft_term_insert_mode_off(&shell->terminal);  /* Désactive le mode insertion */
        }
        if (text->head == text->tail)
        {
            shell->highlighted.texts = ft_highlight_remove_area(shell->highlighted.texts);
        }
        else
        {
            shell->highlighted.texts = ft_highlight_sort_area(shell->highlighted.texts);
        }
        ft_term_clear_modes(&shell->terminal); /* Désactive tous les modes actifs */
    }
    else
    {
        ft_term_highlight_mode_on(&shell->terminal); /* Active le mode surlignage */
        text = ft_highlight_get_area(shell->command.pos, shell->highlighted.texts);
        if (text == NULL)
        {
            shell->highlighted.texts = ft_highlight_new_area(shell->command.pos, shell->highlighted.texts);
        }
        else
        {
            shell->highlighted.texts = text;
        }
    }
}
