/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_key_analyser.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/03/13 17:32:26 by gbourgeo          #+#    #+#             */
/*   Updated: 2018/04/06 18:39:26 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_command.h"
#include "ft_highlight.h"
#include "ft_history.h"
#include "ft_shell.h"
#include "ft_termios.h"
#include "ft_termkeys.h"
#include "libft.h"

/**
 * @brief Traite la commande en cours.
 * @param[in] shell Structure interne du shell
 */
static void ft_treat_command(t_shell *shell)
{
    if (shell->options & SHELL_INTERACTIVE_MODE)
    {
        if (shell->highlighted.on != 0)
        {
            ft_term_clear_modes(&shell->terminal); /* Désactive tous les modes actifs */
            shell->highlighted.on = 0;
        }
        ft_highlight_remove_all(&shell->highlighted.texts);
        ft_move_cursor_end_of_command(shell);
        ft_term_move_cursor_down(&shell->terminal);     /* Déplace le curseur au début de la ligne suivante */
        ft_term_clear_line_and_under(&shell->terminal); /* Efface la ligne et celles sous le curseur */
        shell->prompt.print = 1;
    }
    if (shell->command.buffer_len != 0)
    {
        ft_parse_command(shell);
        shell->history = ft_history_new(shell->command.buffer, shell->history);
        ft_command_reinit(&shell->command);
    }
}

void ft_key_analyser(const char *buffer, ssize_t len, t_shell *shell)
{
    static t_key keys[] = {
        { { 0x03 },                               1, ft_control_c                     }, /* Ctrl + C             */
        { { 0x04 },                               1, ft_control_d                     }, /* Ctrl + D             */
        { { 0x08 },                               1, ft_delete_word_left              }, /* Ctrl + Backspace     */
        { { 0x0A },                               1, ft_treat_command                 }, /* Enter                */
        { { 0x0C },                               1, ft_highlight_mode                }, /* Ctrl + L             */
        { { 0x10 },                               1, ft_highlight_paste               }, /* Ctrl + P             */
        { { 0x19 },                               1, ft_highlight_yank                }, /* Ctrl + Y             */
        // { { 0x1B, 0x5B, 0x31, 0x3b, 0x32, 0x43 }, 6, ft_move_shift_right              }, /* Shift + Right        */
        // { { 0x1B, 0x5B, 0x31, 0x3b, 0x32, 0x44 }, 6, ft_move_shift_left               }, /* Shift + Left         */
        { { 0x1B, 0x5B, 0x31, 0x3B, 0x33, 0x41 }, 6, ft_move_cursor_up                }, /* Alt + Up             */
        { { 0x1B, 0x5B, 0x31, 0x3B, 0x33, 0x42 }, 6, ft_move_cursor_down              }, /* Alt + Down           */
        { { 0x1B, 0x5B, 0x31, 0x3B, 0x33, 0x43 }, 6, ft_move_word_right               }, /* Alt + Right          */
        { { 0x1B, 0x5B, 0x31, 0x3B, 0x33, 0x44 }, 6, ft_move_word_left                }, /* Alt + Left           */
        { { 0x1B, 0x5B, 0x31, 0x3B, 0x35, 0x41 }, 6, ft_move_cursor_up                }, /* Ctrl + Up            */
        { { 0x1B, 0x5B, 0x31, 0x3B, 0x35, 0x42 }, 6, ft_move_cursor_down              }, /* Ctrl + Down          */
        { { 0x1B, 0x5B, 0x31, 0x3B, 0x35, 0x43 }, 6, ft_move_word_right               }, /* Ctrl + Right         */
        { { 0x1B, 0x5B, 0x31, 0x3B, 0x35, 0x44 }, 6, ft_move_word_left                }, /* Ctrl + Left          */
        { { 0x1B, 0x5B, 0x33, 0x3B, 0x33, 0x7E }, 6, ft_delete_word_right             }, /* Alt + Delete         */
        { { 0x1B, 0x5B, 0x33, 0x7E },             4, ft_delete_character_right        }, /* Delete               */
        { { 0x1B, 0x5B, 0x41 },                   3, ft_print_prev_command            }, /* Arrow Up             */
        { { 0x1B, 0x5B, 0x42 },                   3, ft_print_next_command            }, /* Arrow Down           */
        { { 0x1B, 0x5B, 0x43 },                   3, ft_move_cursor_right             }, /* Arrow Right          */
        { { 0x1B, 0x5B, 0x44 },                   3, ft_move_cursor_left              }, /* Arrow Left           */
        { { 0x1B, 0x5B, 0x46 },                   3, ft_move_cursor_end_of_command    }, /* End                  */
        { { 0x1B, 0x5B, 0x48 },                   3, ft_move_cursor_start_of_commmand }, /* Home                 */
        { { 0x1B, 0x68 },                         2, ft_highlight_mode                }, /* Alt + H              */
        { { 0x1B, 0x70 },                         2, ft_highlight_paste               }, /* Alt + P              */
        { { 0x1B, 0x79 },                         2, ft_highlight_yank                }, /* Alt + Y              */
        { { 0x1B, 0x7F },                         2, ft_delete_word_left              }, /* Alt + Backspace      */
        { { 0x7F },                               1, ft_delete_character_left         }, /* Backspace            */
    };
    ssize_t iter;
    size_t  jter;

    iter = 0;
    while (iter < len)
    {
        if (ft_isprint(buffer[iter]))
        {
            ft_insert_character(buffer[iter], shell);
            if (shell->options & SHELL_INTERACTIVE_MODE)
            {
                ft_print_character(shell);
                ft_print_command(shell, 1);
            }
            iter++;
            continue;
        }
        jter = 0;
#pragma unroll 28
        while (jter < sizeof(keys) / sizeof(keys[0]))
        {
            if (ft_memcmp(keys[jter].code, &buffer[iter], keys[jter].code_len) == 0)
            {
                keys[jter].handler(shell);
                iter += keys[jter].code_len - 1;
                break;
            }
            jter++;
        }
        iter++;
    }
}
