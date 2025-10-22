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
#include "ft_shell_key_combination.h"
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
        /* Longueur de 6 */
        { KEYBOARD_CTRL_UP,        ft_move_cursor_up                }, /* Ctrl + Up         */
        { KEYBOARD_CTRL_DOWN,      ft_move_cursor_down              }, /* Ctrl + Down       */
        { KEYBOARD_CTRL_RIGHT,     ft_move_word_right               }, /* Ctrl + Right      */
        { KEYBOARD_CTRL_LEFT,      ft_move_word_left                }, /* Ctrl + Left       */
        { KEYBOARD_CTRL_DELETE,    ft_delete_word_right             }, /* Ctrl + Delete     */
        { KEYBOARD_ALT_UP,         ft_move_cursor_up                }, /* Alt + Up          */
        { KEYBOARD_ALT_DOWN,       ft_move_cursor_down              }, /* Alt + Down        */
        { KEYBOARD_ALT_DELETE,     ft_delete_word_right             }, /* Alt + Delete      */
        /* Longueur de 4 */
        { KEYBOARD_DELETE,         ft_delete_character_right        }, /* Delete            */
        /* Longueur de 3 */
        { KEYBOARD_UP,             ft_print_prev_command            }, /* Arrow Up          */
        { KEYBOARD_DOWN,           ft_print_next_command            }, /* Arrow Down        */
        { KEYBOARD_RIGHT,          ft_move_cursor_right             }, /* Arrow Right       */
        { KEYBOARD_LEFT,           ft_move_cursor_left              }, /* Arrow Left        */
        { KEYBOARD_END,            ft_move_cursor_end_of_command    }, /* End               */
        { KEYBOARD_HOME,           ft_move_cursor_start_of_commmand }, /* Home              */
        /* Longueur de 2 */
        { KEYBOARD_ALT_RIGHT,      ft_move_word_right               }, /* Alt + Right       */
        { KEYBOARD_ALT_LEFT,       ft_move_word_left                }, /* Alt + Left        */
        { KEYBOARD_ALT_H,          ft_highlight_mode                }, /* Alt + H           */
        { KEYBOARD_ALT_P,          ft_highlight_paste               }, /* Alt + P           */
        { KEYBOARD_ALT_Y,          ft_highlight_yank                }, /* Alt + Y           */
        { KEYBOARD_ALT_ENTER,      ft_key_test                      }, /* Alt + Enter       */
        /* Longueur de 1 */
        { KEYBOARD_CTRL_C,         ft_control_c                     }, /* Ctrl + C          */
        { KEYBOARD_CTRL_D,         ft_control_d                     }, /* Ctrl + D          */
        { KEYBOARD_CTRL_BACKSPACE, ft_delete_word_left              }, /* Ctrl + Backspace  */
        { KEYBOARD_CTRL_L,         ft_highlight_mode                }, /* Ctrl + L          */
        { KEYBOARD_CTRL_P,         ft_highlight_paste               }, /* Ctrl + P          */
        { KEYBOARD_CTRL_Y,         ft_highlight_yank                }, /* Ctrl + Y          */
        { KEYBOARD_ALT_BACKSPACE,  ft_delete_word_left              }, /* Alt + Backspace   */
        { KEYBOARD_BACKSPACE,      ft_delete_character_left         }, /* Backspace         */
        { KEYBOARD_ENTER,          ft_shell_exec_command            }, /* Enter             */
        { KEYBOARD_ESC,            ft_clear_modes                   }, /* Esc               */
    };
    size_t jter = 0;

#pragma unroll(_length_of(keys))
    while (jter < _length_of(keys))
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
        size_t found = ft_key_special(buffer, iter, len, shell);
        if (found != 0)
        {
            iter += found;
            continue;
        }
        /* Déplacement des Zones de texte surligné */
        ft_command_highlight_move_areas(shell->command, SHELL_HIGHLIGHTED_AREA_ADD_CHAR, 1);
        /* Insertion du caractère dans le buffer de commande */
        ft_shell_command_insert_character(shell->command, buffer[iter]);
        /* Calcul de la position du curseur courante */
        ft_shell_terminal_calc_current_command_position(&shell->terminal, shell->command->pos);
        /* Calcul de la position du curseur en fin de commande */
        ft_shell_terminal_calc_end_command_position(&shell->terminal, shell->command->len);
        /* Affichage de la commande et déplacement du curseur */
        ft_shell_command_print(shell->command, &shell->terminal, COMMAND_PRINT_FROM_POS_LESS);
        iter++;
    }
}
