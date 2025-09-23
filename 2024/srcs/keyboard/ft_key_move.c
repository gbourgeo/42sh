/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_key_move.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/03/13 16:03:26 by gbourgeo          #+#    #+#             */
/*   Updated: 2018/04/06 18:39:13 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_defines.h"
#include "ft_shell.h"
#include "ft_shell_command.h"
#include "ft_shell_terminal.h"
#include "ft_shell_termkeys.h"
#include "libft.h"
#include <stddef.h>
#include <stdint.h>

void ft_move_cursor_right(t_shell *shell)
{
    t_cmd         *command = shell->command;
    const uint8_t *buffer  = command->buffer;
    size_t         pos     = command->pos;

    if (buffer[pos] == '\0') /* Fin du buffer, pas de déplacement possible */
    {
        return;
    }
    if (TEST_BIT(command->option, COMMAND_HIGHLIGHT_MODE)) /* Mode surlignage actif */
    {
        ft_command_highlight_move_areas(command, SHELL_HIGHLIGHTED_AREA_MOVE_RIGHT, 1);
        ft_shell_command_print(command, &shell->terminal, COMMAND_PRINT_FROM_POS);
    }
    ft_command_change_pos(command, 1, COMMAND_POS_ADD_VALUE, &shell->terminal);
}

void ft_move_cursor_left(t_shell *shell)
{
    t_cmd *command = shell->command;

    if (command->pos == 0)
    {
        return;
    }
    if (TEST_BIT(command->option, COMMAND_HIGHLIGHT_MODE)) /* Mode surlignage actif */
    {
        ft_command_highlight_move_areas(command, SHELL_HIGHLIGHTED_AREA_MOVE_LEFT, 1);
        ft_command_change_pos(command, 1, COMMAND_POS_SUBSTRACT_VALUE, &shell->terminal);
        ft_shell_command_print(command, &shell->terminal, COMMAND_PRINT_FROM_POS);
    }
    else
    {
        ft_command_change_pos(command, 1, COMMAND_POS_SUBSTRACT_VALUE, &shell->terminal);
    }
}

void ft_move_cursor_end_of_command(t_shell *shell)
{
    t_cmd *command = shell->command;

    if (command->pos == command->len)
    {
        return;
    }
    if (TEST_BIT(command->option, COMMAND_HIGHLIGHT_MODE)) /* Mode surlignage actif */
    {
        size_t value = command->len - command->pos;
        ft_command_highlight_move_areas(command, SHELL_HIGHLIGHTED_AREA_MOVE_RIGHT, value);
        ft_shell_command_print(command, &shell->terminal, COMMAND_PRINT_FROM_POS);
    }
    ft_command_change_pos(command,
                          command->len - command->pos,
                          COMMAND_POS_ADD_VALUE,
                          &shell->terminal);
}

void ft_move_cursor_start_of_commmand(t_shell *shell)
{
    t_cmd *command = shell->command;
    size_t pos     = command->pos;

    if (pos == 0)
    {
        return;
    }
    if (TEST_BIT(command->option, COMMAND_HIGHLIGHT_MODE))
    {
        ft_command_highlight_move_areas(command, SHELL_HIGHLIGHTED_AREA_MOVE_LEFT, pos);
        ft_command_change_pos(command,
                              command->pos,
                              COMMAND_POS_SUBSTRACT_VALUE,
                              &shell->terminal);
        ft_shell_command_print(command, &shell->terminal, COMMAND_PRINT_FROM_START);
    }
    else
    {
        ft_command_change_pos(command,
                              command->pos,
                              COMMAND_POS_SUBSTRACT_VALUE,
                              &shell->terminal);
    }
}

void ft_move_word_right(t_shell *shell)
{
    t_cmd   *command = shell->command;
    uint8_t *buffer  = command->buffer;
    size_t   len     = command->len;
    size_t   newpos  = command->pos;
    size_t   value   = 0;

    while (newpos < len
           && ft_iswhitespace((char) buffer[newpos]))
    {
        newpos++;
    }
    while (newpos < len
           && ft_isprint(buffer[newpos])
           && !ft_iswhitespace((char) buffer[newpos]))
    {
        newpos++;
    }
    if (newpos == command->pos)
    {
        return;
    }
    value = newpos - command->pos;
    if (TEST_BIT(command->option, COMMAND_HIGHLIGHT_MODE)) /* Mode surlignage actif */
    {
        ft_command_highlight_move_areas(command, SHELL_HIGHLIGHTED_AREA_MOVE_RIGHT, value);
        ft_shell_command_print(command, &shell->terminal, COMMAND_PRINT_FROM_POS);
    }
    ft_command_change_pos(command,
                          value,
                          COMMAND_POS_ADD_VALUE,
                          &shell->terminal);
}

void ft_move_word_left(t_shell *shell)
{
    t_cmd   *command = shell->command;
    uint8_t *buffer  = command->buffer;
    size_t   newpos  = command->pos;
    size_t   value   = 0;

    while (newpos > 0
           && ft_iswhitespace((char) buffer[newpos - 1]))
    {
        newpos--;
    }
    while (newpos > 0
           && ft_isprint(buffer[newpos - 1])
           && !ft_iswhitespace((char) buffer[newpos - 1]))
    {
        newpos--;
    }
    if (newpos == command->pos)
    {
        return;
    }
    value = command->pos - newpos;
    if (TEST_BIT(command->option, COMMAND_HIGHLIGHT_MODE)) /* Mode surlignage actif */
    {
        ft_command_highlight_move_areas(command, SHELL_HIGHLIGHTED_AREA_MOVE_LEFT, value);
        ft_command_change_pos(command,
                              value,
                              COMMAND_POS_SUBSTRACT_VALUE,
                              &shell->terminal);
        ft_shell_command_print(command, &shell->terminal, COMMAND_PRINT_FROM_POS);
    }
    else
    {
        ft_command_change_pos(command,
                              value,
                              COMMAND_POS_SUBSTRACT_VALUE,
                              &shell->terminal);
    }
}

void ft_move_cursor_up(t_shell *shell)
{
    t_cmd  *command  = shell->command;
    t_term *terminal = &shell->terminal;
    size_t  newpos   = 0;

    if (newpos == 0)
    {
        return;
    }
    newpos = (command->pos > (size_t) terminal->max_column) ?
        (size_t) terminal->max_column :
        command->pos;
    if (TEST_BIT(command->option, COMMAND_HIGHLIGHT_MODE)) /* Mode surlignage actif */
    {
        ft_command_highlight_move_areas(command, SHELL_HIGHLIGHTED_AREA_MOVE_LEFT, newpos);
        ft_command_change_pos(command, newpos, COMMAND_POS_SUBSTRACT_VALUE, terminal);
        ft_shell_command_print(command, terminal, COMMAND_PRINT_FROM_POS);
    }
    else
    {
        ft_command_change_pos(command, newpos, COMMAND_POS_SUBSTRACT_VALUE, terminal);
    }
}

void ft_move_cursor_down(t_shell *shell)
{
    t_cmd  *command  = shell->command;
    t_term *terminal = &shell->terminal;
    size_t  newpos   = 0;

    if (command->pos == command->len)
    {
        return;
    }
    newpos = (newpos > (size_t) terminal->max_column) ?
        (size_t) terminal->max_column :
        command->len - command->pos;
    if (TEST_BIT(command->option, COMMAND_HIGHLIGHT_MODE)) /* Mode surlignage actif */
    {
        ft_command_highlight_move_areas(command, SHELL_HIGHLIGHTED_AREA_MOVE_RIGHT, newpos);
        ft_shell_command_print(command, terminal, COMMAND_PRINT_FROM_POS);
    }
    ft_command_change_pos(command, newpos, COMMAND_POS_ADD_VALUE, terminal);
}
