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

#include "ft_constants.h"
#include "ft_highlight.h"
#include "ft_history.h"
#include "ft_printf.h"
#include "ft_shell.h"
#include "ft_snprintf.h"
#include "ft_termios.h"
#include "ft_termkeys.h"
#include "ft_token.h"
#include "libft.h"
#include <stdlib.h>
#include <unistd.h>

typedef struct __attribute__((aligned(SOFT_ALIGNMENT_CONSTANT))) debug_s
{
    long   lines_printed;
    size_t pos;
    size_t buf_len;
    int    col_max;
    int    line; /* Position de la ligne de commande après affichage des infos */
} debug_t;

static void preexec_print_info(debug_t *dbg, t_shell *shell)
{
    dbg->lines_printed = 0;
    dbg->pos           = shell->command.pos;
    dbg->buf_len       = ft_strlen(shell->command.buffer);
    dbg->col_max       = shell->terminal.max_column + 1;

    ft_term_clear_modes(&shell->terminal); /* Désactive tous les modes actifs */

    /* Positionne le curseur sur la dernière ligne de la commande */
    while ((shell->prompt.len + dbg->pos) / dbg->col_max < (shell->prompt.len + dbg->buf_len) / dbg->col_max)
    {
        ft_term_move_cursor_down(&shell->terminal); /* Positionne le curseur au
                                                       début de la ligne suivante */
        dbg->pos += shell->terminal.max_column;
    }

    /* Si on est sur la dernière ligne du terminal,
     * la ligne de la commande monte d'un cran à cause de l'affichage des infos */
    dbg->line = shell->terminal.current_line;

    ft_term_move_cursor_down(&shell->terminal); /* Positionne le curseur au début
                                                   de la ligne suivante */
    ft_term_clear_line_and_under(
        &shell->terminal);                      /* Efface la ligne courante et celle du dessous */
}

static void postexec_print_info(debug_t *dbg, t_shell *shell)
{
    /* Restaure la ligne de la commande après affichage des infos */
    shell->terminal.current_line = dbg->line;

    /* Restauration de la position du curseur */
    ft_term_move_cursor(&shell->terminal, shell->terminal.current_column, shell->terminal.current_line);

    /* Réactive le mode surlignage si nécessaire */
    if (shell->highlighted.on != 0)
    {
        ft_term_highlight_mode_on(&shell->terminal); /* Active le mode surlignage */
    }
}

static void get_lines_printed(debug_t *dbg, long buff_print, t_term *terminal)
{
    if (dbg->line + dbg->lines_printed >= terminal->max_line)
    {
        dbg->line = dbg->line - 1;
    }
    dbg->lines_printed = dbg->lines_printed + (buff_print / terminal->max_column + 1);
}

void debug_command_line(const char *buf, long ret, t_shell *shell)
{
    debug_t dbg;
    long    buff_print = 0;

    if (!(shell->options & (unsigned int) SHELL_DEBUG_MODE))
    {
        return;
    }

    preexec_print_info(&dbg, shell);

    /* Affiche le contenu du buffer */
    if (buf != NULL)
    {
        int iter = 0;

        buff_print = ft_printf("Buffer [");
        while (iter < ret)
        {
            buff_print += ft_printf(" \033[36m%x\033[0m", buf[iter]);
            iter++;
        }
        while (iter < MAX_KEY_SIZE)
        {
            buff_print += ft_printf(" 0");
            iter++;
        }
        buff_print += ft_printf(" ]");
        get_lines_printed(&dbg, buff_print, &shell->terminal);
    }

    /* Affiche les zones de texte surlignés */
    t_htext *text    = shell->highlighted.texts;
    int      high_nb = 1;

    while (text)
    {
        size_t start = text->head;
        size_t end   = text->tail;

        if (text->head >= text->tail)
        {
            start = text->tail;
            end   = text->head;
        }

        ft_term_move_cursor_down(&shell->terminal); /* Positionne le curseur au
                                                       début de la ligne suivante */

        buff_print = ft_printf("High[%d]: %.*s (head: %ld, tail: %ld)", high_nb, (int) (end - start), shell->command.buffer + start, text->head, text->tail);

        get_lines_printed(&dbg, buff_print, &shell->terminal);

        high_nb++;
        text = text->next;
    }

    /* Affiche la commande */
    ft_term_move_cursor_down(&shell->terminal); /* Positionne le curseur au début
                                                   de la ligne suivante */
    buff_print = ft_printf("Command: \033[36m%s\033[0m (\033[36m%ld\033[0m/%ld)",
                           shell->command.buffer,
                           shell->command.buffer_len,
                           shell->command.buffer_size);

    get_lines_printed(&dbg, buff_print, &shell->terminal);

    /* Affiche la sauvegarde de commande */
    if (shell->command.original)
    {
        ft_term_move_cursor_down(&shell->terminal); /* Positionne le curseur au
                                                       début de la ligne suivante */
        buff_print = ft_printf("  * Saved: %.*s", shell->terminal.max_column - 1, shell->command.original);
        get_lines_printed(&dbg, buff_print, &shell->terminal);
    }

    /* Affiche les infos d'historique */
    if (shell->history)
    {
        t_hist *hist            = ft_history_rewind(shell->history);
        char    hist_buff[4096] = { 0 };
        size_t  hist_len        = 0;
        size_t  hist_pos        = 0;
        /* Taille de l'historique + Position de l'historique courant */
        while (hist)
        {
            hist_len++;
            if (hist == shell->history)
            {
                hist_pos = hist_len;
            }
            hist = hist->next;
        }

        buff_print = ft_snprintf(hist_buff, sizeof(hist_buff), "Hist(%ld):", hist_len);

        hist = shell->history;
        if (hist && hist->prev)
        {
            buff_print += ft_snprintf(hist_buff + buff_print,
                                      sizeof(hist_buff) - buff_print,
                                      "%s[#%ld:%s]",
                                      (hist->prev->prev) ? " ... " : " ",
                                      hist_pos - 1,
                                      hist->prev->command);
        }
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

        ft_term_move_cursor_down(&shell->terminal); /* Positionne le curseur au
                                                       début de la ligne suivante */
        if (buff_print >= shell->terminal.max_column)
        {
            buff_print = write(STDOUT_FILENO, hist_buff, shell->terminal.max_column - 3);
            write(STDOUT_FILENO, " ...", 4);
        }
        else
        {
            buff_print = write(STDOUT_FILENO, hist_buff, buff_print);
        }
        get_lines_printed(&dbg, buff_print, &shell->terminal);
    }

    /* Affiche des infos supplémentaires */
    ft_term_move_cursor_down(&shell->terminal); /* Positionne le curseur au début
                                                   de la ligne suivante */
    buff_print = ft_printf("Line(\033[36m%d\033[0m/%d) Col(\033[36m%d\033[0m/%d) "
                           "High(\033[36m%d\033[0m) Pos(\033[36m%ld\033[0m) "
                           "Prompt(\033[36m%ld\033[0m)",
                           dbg.line,
                           shell->terminal.max_line,
                           shell->terminal.current_column,
                           shell->terminal.max_column,
                           shell->highlighted.on,
                           shell->command.pos,
                           shell->prompt.len);
    get_lines_printed(&dbg, buff_print, &shell->terminal);

    postexec_print_info(&dbg, shell);
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
        info_len = ft_snprintf(info, sizeof(info), "%.*s", first->head - first->tail, shell->command.buffer + first->tail);
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

    if (!(shell->options & (unsigned int) SHELL_DEBUG_MODE))
    {
        return;
    }

    ptr = token;
    while (ptr)
    {
        nb_tokens++; // Nombre total de token
        ptr = ptr->next;
    }
    if (nb_tokens == 0)
    {
        return;
    }
    saved_len = malloc(nb_tokens * sizeof(*saved_len));
    ptr       = token;
    ft_printf("Token list:  TYPE (length) | ...\n");
    start_save = 0;
    nb_tokens  = 0;
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
