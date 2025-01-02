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

#include "ft_highlight.h"
#include "ft_shell.h"
#include "ft_termios.h"
#include "libft.h"
#include <stdlib.h>

void ft_highlight_yank(t_shell *shell)
{
    t_htext *text = NULL;
    char    *yank = NULL;
    size_t   len  = 0;
    int      col  = 0;
    int      line = 0;

    ft_highlight_sort_area(shell->highlighted.texts);
    ft_freestr(&shell->highlighted.yank);
    while (shell->highlighted.texts)
    {
        text = shell->highlighted.texts;
        len  = ft_strlen(shell->highlighted.yank) + (text->head - text->tail) + 2;
        yank = (char *) malloc(len);
        ft_strcpy(yank, shell->highlighted.yank);
        ft_strcat(yank, " ");
        ft_strncat(yank, shell->command.buffer + text->tail, text->head - text->tail);
        yank[len - 1] = '\0';
        ft_freestr(&shell->highlighted.yank);
        shell->highlighted.yank  = yank;
        shell->highlighted.texts = ft_highlight_remove_area(shell->highlighted.texts);
    }
    shell->highlighted.on = 0;

    /* On se déplace au début de la commande */
    col = (int) (shell->terminal.current_column - (shell->command.pos % shell->terminal.max_column));
    if (col < 0)
    {
        col = shell->terminal.max_column + col + 1;
    }
    line = (int) (shell->terminal.current_line - ((shell->command.pos + shell->prompt.len) / (shell->terminal.max_column + 1)));
    ft_term_move_cursor(&shell->terminal, col, line);
    ft_term_clear_modes(&shell->terminal); /* Désactive tous les modes actifs */
    // ft_term_clear_line_and_under(&shell->terminal); /* Efface depuis le curseur jusqu'à la fin du terminal */
    ft_putstr(shell->command.buffer);

    /* On se replace à la position de base */
    ft_term_move_cursor(&shell->terminal, shell->terminal.current_column, shell->terminal.current_line);
}
