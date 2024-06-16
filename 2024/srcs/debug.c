/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/03/26 05:40:21 by gbourgeo          #+#    #+#             */
/*   Updated: 2014/03/27 18:34:28 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

#include "ft_printf.h"
#include "ft_snprintf.h"
#include "ft_termkeys.h"
#include "ft_minishell.h"

void        debug(char *buf, t_shell *shell)
{
    int     lines_printed = 0;

    if (!(shell->options & SHELL_DEBUG_MODE))
        return ;

    ft_tputs("me", shell);                          /* Désactive tous les modes actifs */

    int pos  = shell->command.pos;
    int buf_len  = ft_strlen(shell->command.buffer);
    int col_max = shell->terminal.max_column + 1;

    /* Positionne le curseur sur la dernière ligne de la commande */
    while ((shell->prompt.len + pos) / col_max < (shell->prompt.len + buf_len) / col_max)
    {
        ft_tputs("nw", shell);                      /* Positionne le curseur au début de la ligne suivante */
        pos += shell->terminal.max_column;
    }

    /**
     * Si on est sur la dernière ligne du terminal,
     * la ligne de la commande monte d'un cran à cause de l'affichage des infos
     */
    int line = shell->terminal.current_line;        /* Position de la ligne de commande après affichage des infos */

    ft_tputs("nw", shell);                          /* Positionne le curseur au début de la ligne suivante */
    ft_tputs("cd", shell);                          /* Efface la ligne courante et celle du dessous */

    /* Affiche des infos */
    ft_printf("Line(%d/%d) Col(%d/%d) High(%d) Pos(%ld) Prompt(%ld)",
        line, shell->terminal.max_line,
        shell->terminal.current_column, shell->terminal.max_column,
        shell->highlighted.on,
        shell->command.pos,
        shell->prompt.len);
    if (line + lines_printed >= shell->terminal.max_line)
        line--;
    lines_printed++;

    /* Affiche le contenu du buffer */
    if (buf != NULL)
    {
        ft_tputs("nw", shell);                      /* Positionne le curseur au début de la ligne suivante */

        ft_printf("Buffer [ %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x ]",
        buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7], buf[8],
        buf[9], buf[10], buf[11], buf[12], buf[13], buf[14], buf[15]);

        if (line + lines_printed >= shell->terminal.max_line)
            line--;
        lines_printed++;
    }

    t_high_t    *text = shell->highlighted.texts;
    int         high_nb = 1;

    /* Affiche les zones de texte surlignés */
    while (text)
    {
        size_t start, end;

        if (text->head >= text->tail)
        {
            start = text->tail;
            end = text->head;
        }
        else
        {
            start = text->head;
            end = text->tail;
        }

        ft_tputs("nw", shell);                      /* Positionne le curseur au début de la ligne suivante */

        ft_printf("High[%d]: %.*s (head: %ld, tail: %ld)", high_nb, (int)(end - start),
            shell->command.buffer + start, text->head, text->tail);

        if (line + lines_printed >= shell->terminal.max_line)
            line--;
        lines_printed++;

        high_nb++;
        text = text->next;
    }

    /* Affiche la commande */
    ft_tputs("nw", shell);                      /* Positionne le curseur au début de la ligne suivante */
    int buff_print = ft_printf("Command: %s", shell->command.buffer);

    /* Affiche la sauvegarde de commande */
    if (shell->command.original)
    {
        buff_print = ft_printf(" (Saved: %s)", shell->command.original);
    }
    if (line + lines_printed >= shell->terminal.max_line)
        line--;
    lines_printed += (buff_print / shell->terminal.max_column + 1);

    /* Affiche les infos d'historique */
    if (shell->history)
    {
        t_hist *hist = ft_history_rewind(shell->history);
        char   hist_buff[4096] = {0};
        size_t hist_len = 0;
        size_t hist_pos = 0;
        /* Taille de l'historique + Position de l'historique courant */
        while (hist) { hist_len++; if (hist == shell->history) hist_pos = hist_len; hist=hist->next; }

        buff_print = ft_snprintf(hist_buff, sizeof(hist_buff), "Hist(%ld):", hist_len);

        hist = shell->history;
        if (hist && hist->prev)
            buff_print += ft_snprintf(hist_buff + buff_print,
                                     sizeof(hist_buff) - buff_print,
                                     "%s[#%ld:%s]",
                                     (hist->prev->prev) ? " ... " : " ",
                                     hist_pos - 1,
                                     hist->prev->command);
        hist = shell->history;
        while (hist && buff_print < shell->terminal.max_column)
        {
            buff_print += ft_snprintf(hist_buff + buff_print,
                                     sizeof(hist_buff) - buff_print,
                                     " [%c%ld:%s]",
                                     (shell->command.original != NULL && hist == shell->history) ? '*' : '#',
                                     hist_pos,
                                     hist->command);
            hist_pos++;
            hist = hist->next;
        }

        ft_tputs("nw", shell);                      /* Positionne le curseur au début de la ligne suivante */
        if (buff_print >= shell->terminal.max_column)
        {
            write(STDOUT_FILENO, hist_buff, shell->terminal.max_column - 3);
            write(STDOUT_FILENO, " ...", 4);
        }
        else
        {
            write(STDOUT_FILENO, hist_buff, buff_print);
        }
        if (line + lines_printed >= shell->terminal.max_line)
            line--;
        lines_printed++;
    }

    /* Restaure la ligne de la commande après affichage des infos */
    shell->terminal.current_line = line;

    /* Restauration de la position du curseur */
    ft_tgoto(shell->terminal.current_column, shell->terminal.current_line, shell);

    /* Réactive le mode surlignage si nécessaire */
    if (shell->highlighted.on)
        ft_tputs("mr", shell);                      /* Active le mode surlignage */
}
