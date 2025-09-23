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

#include "ft_defines.h"
#include "ft_printf.h"
#include "ft_shell.h"
#include "ft_shell_command.h"
#include "ft_shell_constants.h"
#include "ft_shell_terminal.h"
#include "ft_snprintf.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

typedef enum debug_function_id
{
    RANDOM_PRINT_INFO,
    BUFFER_PRINT_INFO,
    COMMAND_PRINT_INFO,
    HIGHLIGHTED_TEXT_PRINT_INFO,
    HISTORIC_PRINT_INFO,
    CURSOR_PRINT_INFO,
} funcid_e;

typedef struct _align(4) debug_s
{
    int lines_printed; /* Compteur de ligne écrites */
} debug_t;

static void preexec_print_info(debug_t *dbg, const t_term *terminal)
{
    /* Initialise la structure debug_t */
    dbg->lines_printed = 0;
    /* Désactive tous les modes actifs */
    ft_term_clear_modes(terminal);
    /* Positionne le curseur à la fin de la commande */
    ft_term_move_cursor(terminal, MOVE_CURSOR_END);
}

static void postexec_print_info(debug_t *dbg, t_term *terminal, const t_cmd *command)
{
    if (terminal->end.line + dbg->lines_printed > terminal->max_line)
    {
        int lines = (terminal->end.line + dbg->lines_printed) - terminal->max_line - 1;
        terminal->start.line -= lines;
        terminal->current.line -= lines;
        terminal->end.line -= lines;
    }
    /* Restauration de la position du curseur */
    ft_term_move_cursor(terminal, MOVE_CURSOR_CURRENT);
    /* Réactive le mode surlignage si nécessaire */
    if (TEST_BIT(command->option, COMMAND_HIGHLIGHT_MODE))
    {
        ft_term_highlight_mode_on(terminal); /* Active le mode surlignage */
    }
}

static void add_lines_printed(debug_t *dbg, int char_printed, const t_term *terminal)
{
    dbg->lines_printed = dbg->lines_printed + (char_printed / terminal->max_column) + 1;
}

static void random_print_info(debug_t *dbg, const t_shell *shell)
{
    int buff_print = 0;

    ft_term_move_cursor_down(&shell->terminal);     /* Positionne le curseur au début de la ligne suivante */
    ft_term_clear_line_and_under(&shell->terminal); /* Efface la ligne courante et celles du dessous */
    buff_print = ft_printf("Highlight mode: \033[36m%s\033[0m, Prompt len: \033[36m%ld\033[0m",
                           TEST_BIT(shell->command->option, COMMAND_HIGHLIGHT_MODE) ? "on" : "off",
                           shell->prompt.printed_len);
    add_lines_printed(dbg, buff_print, &shell->terminal);
}

static void buffer_print_info(const uint8_t *buf, long buff_len, debug_t *dbg, const t_shell *shell)
{
    long iter       = 0;
    int  buff_print = 0;

    ft_term_move_cursor_down(&shell->terminal);     /* Positionne le curseur au début de la ligne suivante */
    ft_term_clear_line_and_under(&shell->terminal); /* Efface la ligne courante et celles du dessous */
    buff_print = ft_printf("Buffer [");
    while (iter < buff_len)
    {
        buff_print += ft_printf(" \033[36m%02X\033[0m", buf[iter]);
        iter++;
    }
    buff_print += ft_printf(" ]");
    add_lines_printed(dbg, buff_print, &shell->terminal);
}

static void command_print_info(debug_t *dbg, const t_shell *shell)
{
    int buff_print = 0;

    ft_term_move_cursor_down(&shell->terminal);     /* Positionne le curseur au début de la ligne suivante */
    ft_term_clear_line_and_under(&shell->terminal); /* Efface la ligne courante et celles du dessous */
    buff_print = ft_printf("Command : \033[36m%s\033[0m (len: \033[36m%ld\033[0m/%ld) (pos: \033[36m%ld\033[0m)",
                           shell->command->buffer,
                           shell->command->len,
                           shell->command->size,
                           shell->command->pos);
    add_lines_printed(dbg, buff_print, &shell->terminal);
    /* Affiche la commande originelle de l'historique */
    if (shell->command->origin != NULL)
    {
        ft_term_move_cursor_down(&shell->terminal);     /* Positionne le curseur au début de la ligne suivante */
        ft_term_clear_line_and_under(&shell->terminal); /* Efface la ligne courante et celles du dessous */
        buff_print = ft_printf("Original: %.*s", &shell->terminal, shell->command->origin);
        add_lines_printed(dbg, buff_print, &shell->terminal);
    }
}

static void highlighted_text_print_info(debug_t *dbg, const t_shell *shell)
{
    t_higharea *first      = shell->command->harea;
    t_higharea *harea      = NULL;
    int         high_nb    = 0;
    int         buff_print = 0;

    if (first == NULL)
    {
        return;
    }
    ft_term_move_cursor_down(&shell->terminal);     /* Positionne le curseur au début de la ligne suivante */
    ft_term_clear_line_and_under(&shell->terminal); /* Efface la ligne courante et celles du dessous */
    /* Rembobine au début */
    while (first->prev != NULL)
    {
        first = first->prev;
    }
    /* Calcul du nombre d'éléments */
    harea = first;
    while (harea != NULL)
    {
        high_nb++;
        harea = harea->next;
    }
    buff_print = ft_printf("Highlighted areas: %d", high_nb);
    add_lines_printed(dbg, buff_print, &shell->terminal);

    harea   = first;
    high_nb = 0;
    while (harea != NULL)
    {
        size_t start = harea->tail;
        size_t end   = harea->head;

        if (harea->head < harea->tail)
        {
            start = harea->head;
            end   = harea->tail;
        }
        ft_term_move_cursor_down(&shell->terminal);     /* Positionne le curseur au début de la ligne suivante */
        ft_term_clear_line_and_under(&shell->terminal); /* Efface la ligne courante et celles du dessous */
        buff_print = ft_printf("    [%d]: %.*s (tail: %ld, head: %ld)",
                               high_nb,
                               (int) (end - start),
                               shell->command->buffer + start,
                               harea->tail,
                               harea->head);
        add_lines_printed(dbg, buff_print, &shell->terminal);
        high_nb++;
        harea = harea->next;
    }
}

static void historic_print_info(debug_t *dbg, const t_shell *shell)
{
    t_cmd *cmd        = ft_shell_command_get_first(shell->command);
    t_cmd *hist       = shell->command->next;
    char   buff[1024] = { 0 };
    long   hist_pos   = 0;
    int    buff_print = 0;

    ft_term_move_cursor_down(&shell->terminal);     /* Positionne le curseur au début de la ligne suivante */
    ft_term_clear_line_and_under(&shell->terminal); /* Efface la ligne courante et celles du dessous */
    /* Position de la commande courante dans l'historique */
    if (cmd->prev != NULL)
    {
        while (hist != NULL)
        {
            hist_pos++;
            if (cmd == hist)
            {
                break;
            }
            hist = hist->next;
        }
    }
    /* Remplissage du buffer jusqu'à terminal.max_column */
    buff_print = ft_snprintf(buff, sizeof(buff), "Hist(%ld):", shell->command_size);
    /* Affichage de l'historique précédent */
    if (hist_pos > 1 && buff_print < shell->terminal.max_column)
    {
        buff_print += ft_snprintf(buff + buff_print,
                                  sizeof(buff) - (size_t) buff_print,
                                  " ... [#%ld: %s]",
                                  hist_pos - 1,
                                  cmd->prev->buffer);
    }
    if (cmd->prev == NULL)
    {
        cmd      = cmd->next;
        hist_pos = 1;
    }
    /* Affichage de l'historique */
    while (cmd != NULL && buff_print < shell->terminal.max_column)
    {
        buff_print += ft_snprintf(buff + buff_print,
                                  sizeof(buff) - (size_t) buff_print,
                                  " [%c%ld: %s]",
                                  (cmd == shell->command) ? '*' : '#',
                                  hist_pos,
                                  cmd->buffer);
        hist_pos++;
        cmd = cmd->next;
    }
    /* Affichage du buffer */
    if (buff_print >= shell->terminal.max_column)
    {
        buff_print = (int) write(STDOUT_FILENO, buff, (size_t) shell->terminal.max_column - 4);
        write(STDOUT_FILENO, " ...", 4);
    }
    else
    {
        buff_print = (int) write(STDOUT_FILENO, buff, (size_t) buff_print);
    }
    add_lines_printed(dbg, buff_print, &shell->terminal);
}

static void cursor_print_info(debug_t *dbg, const t_shell *shell)
{
    int buff_print = 0;
    int line       = 0;

    ft_term_move_cursor_down(&shell->terminal);     /* Positionne le curseur au début de la ligne suivante */
    ft_term_clear_line_and_under(&shell->terminal); /* Efface la ligne courante et celles du dessous */
    line = shell->terminal.start.line;
    if (shell->terminal.end.line + dbg->lines_printed + 3 > shell->terminal.max_line)
    {
        line -= ((shell->terminal.end.line + dbg->lines_printed + 3) - shell->terminal.max_line);
    }
    buff_print = ft_printf("command Start : Line(\033[36m%d\033[0m/%d) Col(\033[36m%d\033[0m/%d)",
                           line,
                           shell->terminal.max_line,
                           shell->terminal.start.column,
                           shell->terminal.max_column);
    add_lines_printed(dbg, buff_print, &shell->terminal);

    ft_term_move_cursor_down(&shell->terminal);     /* Positionne le curseur au début de la ligne suivante */
    ft_term_clear_line_and_under(&shell->terminal); /* Efface la ligne courante et celles du dessous */
    line = shell->terminal.current.line;
    if (shell->terminal.end.line + dbg->lines_printed + 2 > shell->terminal.max_line)
    {
        line -= ((shell->terminal.end.line + dbg->lines_printed + 2) - shell->terminal.max_line);
    }
    buff_print = ft_printf("Cursor Current: Line(\033[36m%d\033[0m/%d) Col(\033[36m%d\033[0m/%d)",
                           line,
                           shell->terminal.max_line,
                           shell->terminal.current.column,
                           shell->terminal.max_column);
    add_lines_printed(dbg, buff_print, &shell->terminal);

    ft_term_move_cursor_down(&shell->terminal);     /* Positionne le curseur au début de la ligne suivante */
    ft_term_clear_line_and_under(&shell->terminal); /* Efface la ligne courante et celles du dessous */
    line = shell->terminal.current.line;
    if (shell->terminal.end.line + dbg->lines_printed + 1 > shell->terminal.max_line)
    {
        line -= ((shell->terminal.end.line + dbg->lines_printed + 1) - shell->terminal.max_line);
    }
    buff_print = ft_printf("Command End   : Line(\033[36m%d\033[0m/%d) Col(\033[36m%d\033[0m/%d)",
                           line,
                           shell->terminal.max_line,
                           shell->terminal.end.column,
                           shell->terminal.max_column);
    add_lines_printed(dbg, buff_print, &shell->terminal);
}

void ft_shell_command_debug(const uint8_t *buffer, long size)
{
    extern t_shell g_shell;
    debug_t        dbg;

    if (!TEST_BIT(g_shell.options, SHELL_DEBUG_MODE))
    {
        return;
    }
    preexec_print_info(&dbg, &g_shell.terminal);
    /* Affiche les informatiosn "random" comme le highlight actif ou la longueur du prompt */
    random_print_info(&dbg, &g_shell);
    /* Affiche le contenu du buffer au format hexadecimal */
    buffer_print_info(buffer, size, &dbg, &g_shell);
    /* Affiche les informations de la commande en cours */
    command_print_info(&dbg, &g_shell);
    /* Affiche les informations des zones de texte surlignés */
    highlighted_text_print_info(&dbg, &g_shell);
    /* Affiche les informations d'historique */
    historic_print_info(&dbg, &g_shell);
    /* Affiche des infos sur les positions du curseur */
    cursor_print_info(&dbg, &g_shell);
    postexec_print_info(&dbg, &g_shell.terminal, g_shell.command);
}
