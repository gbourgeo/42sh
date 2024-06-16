/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_key_read.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/03/26 05:40:21 by gbourgeo          #+#    #+#             */
/*   Updated: 2014/03/27 18:34:28 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_termkeys.h"
#include "ft_minishell.h"

static void ft_treat_command(void *shell_ctx)
{
    t_shell *shell;

    shell = (t_shell *)shell_ctx;
    if (shell->highlighted.on)
    {
        ft_tputs("me", shell); /* Désactive tous les modes actifs */
        shell->highlighted.on = 0;
    }
    ft_highlight_remove_all(&shell->highlighted.texts);
    ft_move_cursor_end_of_command(shell);
    ft_tputs("nw", shell); /* Déplace le curseur au début de la ligne suivante */
    ft_tputs("cd", shell); /* Efface la ligne et celles sous le curseur */
    if (shell->command.buffer_len != 0)
    {
        ft_parse_command(shell);
        shell->history = ft_history_new(shell->command.buffer, shell->history);
        ft_command_reinit(&shell->command);
    }
    shell->prompt.print = 1;
}

/**
 * @brief Fonction d'analyse de caractères du clavier lus (caractère simple ou
 * combinaison de caractère).
 * @param[in] buffer Buffer de réception
 * @param[in] len Nombre de caractère à lire
 * @param[in] shell_ctx Structure d'environnement du shell
*/
void        ft_key_read(char *buffer, int len, void *shell_ctx)
{
    t_key   keys[] = {
        { { 0x03 },                               1, ft_control_c },                        /* Ctrl + C         */
        { { 0x04 },                               1, ft_control_d },                        /* Ctrl + D         */
        { { 0x0A },                               1, ft_treat_command },                    /* Enter            */
        // { { 0x1B, 0x5B, 0x31, 0x3b, 0x32, 0x43 }, 6, ft_move_shift_right },                 /* Shift + Right    */
        // { { 0x1B, 0x5B, 0x31, 0x3b, 0x32, 0x44 }, 6, ft_move_shift_left },                  /* Shift + Left     */
        { { 0x1B, 0x5B, 0x31, 0x3B, 0x35, 0x41 }, 6, ft_move_cursor_up },                   /* Ctrl + Up        */
        { { 0x1B, 0x5B, 0x31, 0x3B, 0x35, 0x42 }, 6, ft_move_cursor_down },                 /* Ctrl + Down      */
        { { 0x1B, 0x5B, 0x31, 0x3B, 0x35, 0x43 }, 6, ft_move_word_right },                  /* Ctrl + Right     */
        { { 0x1B, 0x5B, 0x31, 0x3B, 0x35, 0x44 }, 6, ft_move_word_left },                   /* Ctrl + Left      */
        { { 0x1B, 0x5B, 0x33, 0x3B, 0x33, 0x7E }, 6, ft_delete_word_right },                /* Alt + Delete     */
        { { 0x1B, 0x5B, 0x33, 0x7E },             4, ft_delete_character_right },           /* Delete           */
        { { 0x1B, 0x5B, 0x41 },                   3, ft_print_prev_command },               /* Arrow Up         */
        { { 0x1B, 0x5B, 0x42 },                   3, ft_print_next_command },               /* Arrow Down       */
        { { 0x1B, 0x5B, 0x43 },                   3, ft_move_cursor_right },                /* Arrow Right      */
        { { 0x1B, 0x5B, 0x44 },                   3, ft_move_cursor_left },                 /* Arrow Left       */
        { { 0x1B, 0x5B, 0x46 },                   3, ft_move_cursor_end_of_command },       /* End              */
        { { 0x1B, 0x5B, 0x48 },                   3, ft_move_cursor_start_of_commmand },    /* Home             */
        { { 0x1B, 0x68 },                         2, ft_highlight_mode },                   /* Alt + H          */
        { { 0x1B, 0x70 },                         2, ft_highlight_paste },                  /* Alt + P          */
        { { 0x1B, 0x79 },                         2, ft_highlight_yank },                   /* Alt + Y          */
        { { 0x1B, 0x7F },                         2, ft_delete_word_left },                 /* Alt + Backspace  */
        { { 0x7F },                               1, ft_delete_character_left },            /* Backspace        */
    };
    size_t  j;
    int     i;

    i = 0;
    while (i < len)
    {
        j = 0;
        while (j < sizeof(keys) / sizeof(keys[0]))
        {
            if (ft_memcmp(keys[j].code, &buffer[i], keys[j].code_len) == 0)
            {
                (void) keys[j].handler(shell_ctx);
                i += keys[j].code_len - 1;
                break ;
            }
            j++;
        }
        if (j == sizeof(keys) / sizeof(keys[0]) && ft_isprint(buffer[i]))
        {
            ft_insert_character(buffer[i], shell_ctx);
            ft_print_character(shell_ctx);
            ft_print_command(shell_ctx, 1);
        }
        i++;
    }
}
