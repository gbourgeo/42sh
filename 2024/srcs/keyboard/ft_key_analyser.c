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

#include "ft_defines.h"
#include "ft_shell.h"
#include "ft_shell_command.h"
#include "ft_shell_constants.h"
#include "ft_shell_terminal.h"
#include "ft_shell_termkeys.h"
#include "libft.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static void ft_key_test(t_shell *shell _unused)
{
    // ft_term_clear_cursor_and_under(&shell->terminal);
}

/**
 * @brief Traitement des combinaisons et touches spéciales du clavier.
 * @param buffer Buffer de réception des touches du clavier
 * @param iter Position dans le buffer
 * @param len Taille du buffer
 * @param shell Contexte du shell
 * @return 0 si aucune combinaison supportée n'a été détecté,
 * la taille de la combinaison spéciale détectée autrement.
 */
static size_t ft_key_special(const uint8_t *buffer, size_t iter, size_t size, t_shell *shell)
{
    static t_key keys[] = {
        { { 0x03 },                               1, ft_control_c                     }, /* Ctrl + C             */
        { { 0x04 },                               1, ft_control_d                     }, /* Ctrl + D             */
        { { 0x08 },                               1, ft_delete_word_left              }, /* Ctrl + Backspace     */
        { { 0x0A },                               1, ft_shell_command_exec            }, /* Enter                */
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
#if defined(__APPLE__) || defined(__MACH__)
        { { 0x17 },                               1, ft_delete_word_left              }, /* Alt + Backspace      */
        { { 0x1B, 0x62 },                         2, ft_move_word_left                }, /* Alt + Left           */
        { { 0x1B, 0x5b, 0x33, 0x3b, 0x35, 0x7e }, 6, ft_delete_word_right             }, /* Ctrl + Delete        */
        { { 0x1B, 0x66 },                         2, ft_move_word_right               }, /* Alt + Right          */
        { { 0x1B, 0x0A },                         2, ft_key_test                      }, /* Alt + Enter          */
        { { 0x1B },                               1, ft_clear_modes                   }, /* Esc                  */
#endif
    };
    size_t jter = 0;

#pragma unroll(LENGTH_OF(keys))
    while (jter < LENGTH_OF(keys))
    {
        if (iter + keys[jter].code_len <= size
            && ft_memcmp(keys[jter].code, &buffer[iter], keys[jter].code_len) == 0)
        {
            keys[jter].handler(shell);
            return (keys[jter].code_len);
        }
        jter++;
    }
    return (0);
}

void ft_key_analyser(const uint8_t *buffer, size_t len, t_shell *shell)
{
    size_t iter = 0;

    while (iter < len)
    {
        /* Est-ce une combinaison de touche spéciale ? */
        if (TEST_BIT(shell->options, SHELL_INTERACTIVE_MODE))
        {
            size_t found = ft_key_special(buffer, iter, len, shell);
            if (found != 0)
            {
                iter += found;
                continue;
            }
            ft_command_highlight_move_areas(shell->command, SHELL_HIGHLIGHTED_AREA_ADD_CHAR, 1);
        }
        /* Insertion du caractère dans le buffer de commande */
        ft_shell_command_insert_character(shell->command, buffer[iter]);
        /* Déplacement du curseur */
        if (TEST_BIT(shell->options, SHELL_INTERACTIVE_MODE))
        {
            ft_shell_terminal_calc_current_command_position(&shell->terminal, shell->command->pos);
            ft_shell_terminal_calc_end_command_position(&shell->terminal, shell->command->len);
            ft_shell_command_print(shell->command, &shell->terminal, UINT32(COMMAND_PRINT_FROM_POS_LESS));
        }
        iter++;
    }
}
