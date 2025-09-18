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
#include "ft_shell_log.h"
#include "ft_shell_terminal.h"
#include "ft_shell_token.h"
#include "ft_snprintf.h"
#include "libft.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct _align(64) debug_s
{
    long   lines_printed; /* Compteur de ligne écrites */
    size_t pos;           /* Sauvegarde de la position d'écriture du buffer */
    size_t buf_len;       /* Sauvegarde de la taille du buffer */
    int    col_max;       /* Sauvegarde du nombre de colonne du terminal */
    int    start_line;    /* Position de la ligne de commande après affichage des infos */
    int    current_line;  /* Position de la ligne de commande après affichage des infos */
    int    end_line;      /* Position de la ligne de commande après affichage des infos */
} debug_t;

static void preexec_print_info(debug_t *dbg, t_shell *shell)
{
    dbg->lines_printed = 0;
    dbg->pos           = shell->command->pos;
    dbg->buf_len       = ft_strlen((const char *) shell->command->buffer);
    dbg->col_max       = shell->terminal.max_column;

    ft_term_clear_modes(&shell->terminal); /* Désactive tous les modes actifs */

    /* Positionne le curseur sur la dernière ligne de la commande */
    ft_term_move_cursor(&shell->terminal, MOVE_CURSOR_END);
    // while ((shell->prompt.printed_len + dbg->pos) / (size_t) dbg->col_max
    //        < (shell->prompt.printed_len + dbg->buf_len) / (size_t) dbg->col_max)
    // {
    //     ft_term_move_cursor_down(&shell->terminal); /* Positionne le curseur au début de la ligne suivante */
    //     dbg->pos += (size_t) shell->terminal.max_column;
    // }

    /* Si on est sur la dernière ligne du terminal,
     * la ligne de la commande monte d'un cran à
     * cause de l'affichage des infos */
    dbg->start_line   = shell->terminal.start.line;
    dbg->current_line = shell->terminal.current.line;
    dbg->end_line     = shell->terminal.end.line;
}

static void postexec_print_info(debug_t *dbg, t_term *terminal, t_cmd *command)
{
    /* Restaure la ligne de la commande après affichage des infos */
    terminal->start.line   = dbg->start_line;
    terminal->current.line = dbg->current_line;
    terminal->end.line     = dbg->end_line;

    if (dbg->end_line + dbg->lines_printed >= terminal->max_line)
    {
        dbg->start_line -= ((dbg->end_line + (int) dbg->lines_printed) - terminal->max_line);
        dbg->current_line -= ((dbg->end_line + (int) dbg->lines_printed) - terminal->max_line);
        dbg->end_line -= ((dbg->end_line + (int) dbg->lines_printed) - terminal->max_line);
    }

    /* Restauration de la position du curseur */
    ft_term_move_cursor(terminal, MOVE_CURSOR_CURRENT);

    /* Réactive le mode surlignage si nécessaire */
    if (TEST_BIT(command->option, COMMAND_HIGHLIGHT_MODE))
    {
        ft_term_highlight_mode_on(terminal); /* Active le mode surlignage */
    }
}

static void get_lines_printed(debug_t *dbg, long buff_print, t_term *terminal)
{
    if (dbg->end_line + dbg->lines_printed >= terminal->max_line)
    {
        dbg->start_line   = dbg->start_line - ((dbg->end_line + (int) dbg->lines_printed) - terminal->max_line);
        dbg->current_line = dbg->current_line - ((dbg->end_line + (int) dbg->lines_printed) - terminal->max_line);
        dbg->end_line     = dbg->end_line - ((dbg->end_line + (int) dbg->lines_printed) - terminal->max_line);
    }
    dbg->lines_printed = dbg->lines_printed + (buff_print / terminal->max_column);
}

static void random_print_info(debug_t *dbg, t_shell *shell)
{
    long buff_print = 0;

    ft_term_move_cursor_down(&shell->terminal);     /* Positionne le curseur au début de la ligne suivante */
    ft_term_clear_line_and_under(&shell->terminal); /* Efface la ligne courante et celles du dessous */
    buff_print = ft_printf("Highlight mode: \033[36m%s\033[0m, Prompt len: \033[36m%ld\033[0m",
                           TEST_BIT(shell->command->option, COMMAND_HIGHLIGHT_MODE) ? "on" : "off",
                           shell->prompt.printed_len);
    get_lines_printed(dbg, buff_print, &shell->terminal);
}

static void buffer_print_info(const uint8_t *buf, long buff_len, debug_t *dbg, t_shell *shell)
{
    long iter       = 0;
    long buff_print = 0;

    ft_term_move_cursor_down(&shell->terminal);     /* Positionne le curseur au début de la ligne suivante */
    ft_term_clear_line_and_under(&shell->terminal); /* Efface la ligne courante et celles du dessous */
    buff_print = ft_printf("Buffer [");
    while (iter < buff_len)
    {
        buff_print += ft_printf(" \033[36m%02X\033[0m", buf[iter]);
        iter++;
    }
    buff_print += ft_printf(" ]");
    get_lines_printed(dbg, buff_print, &shell->terminal);
}

static void command_print_info(debug_t *dbg, t_shell *shell)
{
    long buff_print = 0;

    ft_term_move_cursor_down(&shell->terminal);     /* Positionne le curseur au début de la ligne suivante */
    ft_term_clear_line_and_under(&shell->terminal); /* Efface la ligne courante et celles du dessous */
    buff_print = ft_printf("Command : \033[36m%s\033[0m (len: \033[36m%ld\033[0m/%ld) (pos: \033[36m%ld\033[0m)",
                           shell->command->buffer,
                           shell->command->len,
                           shell->command->size,
                           shell->command->pos);
    get_lines_printed(dbg, buff_print, &shell->terminal);
    /* Affiche la commande originelle de l'historique */
    if (shell->command->origin != NULL)
    {
        ft_term_move_cursor_down(&shell->terminal);     /* Positionne le curseur au début de la ligne suivante */
        ft_term_clear_line_and_under(&shell->terminal); /* Efface la ligne courante et celles du dessous */
        buff_print = ft_printf("Original: %.*s", shell->terminal.max_column, shell->command->origin);
        get_lines_printed(dbg, buff_print, &shell->terminal);
    }
}

static void highlighted_text_print_info(debug_t *dbg, t_shell *shell)
{
    t_higharea *first      = shell->command->harea;
    t_higharea *harea      = NULL;
    int         high_nb    = 0;
    long        buff_print = 0;

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
    get_lines_printed(dbg, buff_print, &shell->terminal);

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
        get_lines_printed(dbg, buff_print, &shell->terminal);
        high_nb++;
        harea = harea->next;
    }
}

static void historic_print_info(debug_t *dbg, t_shell *shell)
{
    t_cmd *cmd        = ft_shell_command_get_first(shell->command);
    t_cmd *hist       = shell->command->next;
    char   buff[1024] = { 0 };
    long   hist_pos   = 0;
    long   buff_print = 0;

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
        cmd = cmd->next;
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
        buff_print = write(STDOUT_FILENO, buff, (size_t) shell->terminal.max_column - 3);
        write(STDOUT_FILENO, " ...", 4);
    }
    else
    {
        buff_print = write(STDOUT_FILENO, buff, (size_t) buff_print);
    }
    get_lines_printed(dbg, buff_print, &shell->terminal);
}

static void cursor_print_info(debug_t *dbg, t_shell *shell)
{
    long buff_print = 0;
    int  line       = 0;

    ft_term_move_cursor_down(&shell->terminal);     /* Positionne le curseur au début de la ligne suivante */
    ft_term_clear_line_and_under(&shell->terminal); /* Efface la ligne courante et celles du dessous */
    line = shell->terminal.start.line;
    if (shell->terminal.end.line + dbg->lines_printed + 3 > shell->terminal.max_line)
    {
        line -= ((shell->terminal.end.line + (int) dbg->lines_printed + 3) - shell->terminal.max_line);
    }
    buff_print = ft_printf("command Start : Line(\033[36m%d\033[0m/%d) Col(\033[36m%d\033[0m/%d)",
                           line,
                           shell->terminal.max_line,
                           shell->terminal.start.column,
                           shell->terminal.max_column);
    get_lines_printed(dbg, buff_print, &shell->terminal);

    ft_term_move_cursor_down(&shell->terminal);     /* Positionne le curseur au début de la ligne suivante */
    ft_term_clear_line_and_under(&shell->terminal); /* Efface la ligne courante et celles du dessous */
    line = shell->terminal.current.line;
    if (shell->terminal.end.line + dbg->lines_printed + 2 > shell->terminal.max_line)
    {
        line -= ((shell->terminal.end.line + (int) dbg->lines_printed + 2) - shell->terminal.max_line);
    }
    buff_print = ft_printf("Cursor Current: Line(\033[36m%d\033[0m/%d) Col(\033[36m%d\033[0m/%d)",
                           line,
                           shell->terminal.max_line,
                           shell->terminal.current.column,
                           shell->terminal.max_column);
    get_lines_printed(dbg, buff_print, &shell->terminal);

    ft_term_move_cursor_down(&shell->terminal);     /* Positionne le curseur au début de la ligne suivante */
    ft_term_clear_line_and_under(&shell->terminal); /* Efface la ligne courante et celles du dessous */
    line = shell->terminal.current.line;
    if (shell->terminal.end.line + dbg->lines_printed + 1 > shell->terminal.max_line)
    {
        line -= ((shell->terminal.end.line + (int) dbg->lines_printed + 1) - shell->terminal.max_line);
    }
    buff_print = ft_printf("Command End   : Line(\033[36m%d\033[0m/%d) Col(\033[36m%d\033[0m/%d)",
                           line,
                           shell->terminal.max_line,
                           shell->terminal.end.column,
                           shell->terminal.max_column);
    get_lines_printed(dbg, buff_print, &shell->terminal);
}

void debug_command_line(const uint8_t *buf, long ret, t_shell *shell)
{
    debug_t dbg;

    if (!TEST_BIT(shell->options, SHELL_DEBUG_MODE))
    {
        return;
    }

    preexec_print_info(&dbg, shell);

    /* Affiche les informatiosn "random" comme le highlight actif ou la longueur du prompt */
    random_print_info(&dbg, shell);
    /* Affiche le contenu du buffer au format hexadecimal */
    buffer_print_info(buf, ret, &dbg, shell);
    /* Affiche les informations de la commande en cours */
    command_print_info(&dbg, shell);
    /* Affiche les informations des zones de texte surlignés */
    highlighted_text_print_info(&dbg, shell);
    /* Affiche les informations d'historique */
    historic_print_info(&dbg, shell);
    /* Affiche des infos sur les positions du curseur */
    cursor_print_info(&dbg, shell);

    postexec_print_info(&dbg, &shell->terminal, shell->command);
}

static t_token *debug_tokens_print_token(t_token *first, t_token *last,
                                         const long *saved_len, t_shell *shell)
{
    char info[256] = { 0 };
    long info_len  = 0;
    int  nb_tokens = 0;

    ft_printf("\n");
    while (first != last)
    {
        info_len = ft_snprintf(info, sizeof(info), "%.*s", first->head - first->tail, shell->command->buffer + first->tail);
        for (long i = info_len; i < saved_len[nb_tokens]; i++)
        {
            ft_strncat(info, " ", sizeof(info));
        }
        ft_printf("\033[36m%s%s\033[0m", (nb_tokens == 0) ? "  " : "   ", info);
        nb_tokens++;
        first = first->next;
    }
    ft_printf("\n");
    return (first);
}

void debug_tokens(t_token *token, t_shell *shell)
{
    char     info[256]  = { 0 };
    long     info_len   = 0;
    t_token *ptr        = NULL;
    int      start_save = 0;
    int      nb_tokens  = 0;
    int      printed    = 0;
    long    *saved_len  = NULL;

    if (!TEST_BIT(shell->options, SHELL_DEBUG_MODE))
    {
        return;
    }
    ft_log(SH_LOG_LEVEL_DBG, "Line: %s", shell->command->buffer);
    // Calcul du nombre total de token
    ptr = token;
    while (ptr)
    {
        nb_tokens++;
        ptr = ptr->next;
    }
    if (nb_tokens == 0)
    {
        return;
    }
    saved_len = malloc((size_t) nb_tokens * sizeof(*saved_len));
    ptr       = token;
    ft_log(SH_LOG_LEVEL_DBG, "Token list:  TYPE (length) | ...");
    nb_tokens = 0;
    while (token)
    {
        info_len = ft_snprintf(info, sizeof(info), "%s (%ld)", ft_token_type_to_str(token->type), token->head - token->tail);

        if (printed + info_len + 3 < shell->terminal.max_column)
        {
            saved_len[nb_tokens] = (info_len > (long) (token->head - token->tail)) ? info_len : (long) (token->head - token->tail);
            for (long i = info_len; i < saved_len[nb_tokens]; i++)
            {
                ft_strncat(info, " ", sizeof(info));
            }
            printed += ft_printf("%s%s", (token == ptr) ? "  " : " | ", info);
            nb_tokens++;
            token = token->next;
        }
        else                  // On est arrivé à écrire jusqu'à la fin du terminal
        {
            if (ptr == token) // On a rien écrit, terminal trop petit, on quitte
            {
                ft_printf(" \033[31mAborted\033[0m\n");
                break;
            }
            ptr        = debug_tokens_print_token(ptr, token, saved_len + start_save, shell);
            start_save = nb_tokens;
            printed    = 0;
        }
    }
    debug_tokens_print_token(ptr, token, saved_len + start_save, shell);
    free(saved_len);
}
