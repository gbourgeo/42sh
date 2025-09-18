/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_key_suppr.c                                     :+:      :+:    :+:   */
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

void ft_delete_character_right(t_shell *shell)
{
    t_cmd *command = shell->command;

    if (command->pos < command->len)
    {
        /* Shell mode intéractif */
        if (TEST_BIT(shell->options, SHELL_INTERACTIVE_MODE))
        {
            /* Décalage des zones de textes surlignées */
            ft_command_highlight_move_areas(command, SHELL_HIGHLIGHTED_AREA_REMOVE_CHAR_RIGHT, 1);
        }
        /* Supprime un caractère à la position du buffer */
        ft_shell_command_delete_character(command, COMMAND_REMOVE_CHAR_RIGHT, 1);
        if (TEST_BIT(shell->options, SHELL_INTERACTIVE_MODE))
        {
            ft_shell_terminal_calc_end_command_position(&shell->terminal, command->len);
            ft_shell_command_print(shell->command, &shell->terminal, UINT32(COMMAND_PRINT_FROM_POS));
        }
    }
}

void ft_delete_character_left(t_shell *shell)
{
    t_cmd *command = shell->command;

    if (command->pos > 0)
    {
        /* Shell mode intéractif */
        if (TEST_BIT(shell->options, SHELL_INTERACTIVE_MODE))
        {
            /* Décalage des zones de textes surlignées */
            ft_command_highlight_move_areas(command, SHELL_HIGHLIGHTED_AREA_REMOVE_CHAR_LEFT, 1);
        }
        /* Supprime un caractère à gauche de la position du buffer */
        ft_shell_command_delete_character(command, COMMAND_REMOVE_CHAR_LEFT, 1);
        if (TEST_BIT(shell->options, SHELL_INTERACTIVE_MODE))
        {
            ft_shell_terminal_calc_current_command_position(&shell->terminal, command->pos);
            ft_shell_terminal_calc_end_command_position(&shell->terminal, command->len);
            ft_term_move_cursor(&shell->terminal, MOVE_CURSOR_CURRENT);
            ft_shell_command_print(shell->command, &shell->terminal, UINT32(COMMAND_PRINT_FROM_POS));
        }
    }
}

void ft_delete_word_right(t_shell *shell)
{
    t_cmd   *command = shell->command;
    uint8_t *buffer  = command->buffer;
    size_t   pos     = command->pos;
    size_t   value   = 0;

    while (ft_iswhitespace((char) buffer[pos]))
    {
        pos++;
    }
    while (ft_isprint(buffer[pos])
           && !ft_iswhitespace((char) buffer[pos]))
    {
        pos++;
    }
    value = pos - command->pos;
    if (value > 0)
    {
        /* Shell mode intéractif */
        if (TEST_BIT(shell->options, SHELL_INTERACTIVE_MODE))
        {
            /* Décalage des zones de textes surlignées */
            ft_command_highlight_move_areas(command, SHELL_HIGHLIGHTED_AREA_REMOVE_CHAR_RIGHT, value);
        }
        /* Supprime 'value' caractères à gauche de la position du buffer */
        ft_shell_command_delete_character(command, COMMAND_REMOVE_CHAR_RIGHT, value);
        if (TEST_BIT(shell->options, SHELL_INTERACTIVE_MODE))
        {
            ft_shell_terminal_calc_current_command_position(&shell->terminal, command->pos);
            ft_shell_terminal_calc_end_command_position(&shell->terminal, command->len);
            ft_term_move_cursor(&shell->terminal, MOVE_CURSOR_CURRENT);
            ft_shell_command_print(shell->command, &shell->terminal, UINT32(COMMAND_PRINT_FROM_POS));
        }
    }
}

void ft_delete_word_left(t_shell *shell)
{
    t_cmd   *command = shell->command;
    uint8_t *buffer  = command->buffer;
    size_t   pos     = command->pos;
    size_t   value   = 0;

    while (pos > 0 && ft_iswhitespace((char) buffer[pos - 1]))
    {
        pos--;
    }
    while (pos > 0
           && ft_isprint(buffer[pos - 1])
           && !ft_iswhitespace((char) buffer[pos - 1]))
    {
        pos--;
    }
    value = command->pos - pos;
    if (value > 0)
    {
        /* Shell mode intéractif */
        if (TEST_BIT(shell->options, SHELL_INTERACTIVE_MODE))
        {
            /* Décalage des zones de textes surlignées */
            ft_command_highlight_move_areas(command, SHELL_HIGHLIGHTED_AREA_REMOVE_CHAR_LEFT, value);
        }
        /* Supprime un caractère à gauche de la position du buffer */
        ft_shell_command_delete_character(command, COMMAND_REMOVE_CHAR_LEFT, value);
        if (TEST_BIT(shell->options, SHELL_INTERACTIVE_MODE))
        {
            ft_shell_terminal_calc_current_command_position(&shell->terminal, command->pos);
            ft_shell_terminal_calc_end_command_position(&shell->terminal, command->len);
            ft_term_move_cursor(&shell->terminal, MOVE_CURSOR_CURRENT);
            ft_shell_command_print(shell->command, &shell->terminal, UINT32(COMMAND_PRINT_FROM_POS));
        }
    }
}
