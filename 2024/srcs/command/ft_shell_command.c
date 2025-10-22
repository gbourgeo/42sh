/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_shell_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/02/12 22:32:55 by gbourgeo          #+#    #+#             */
/*   Updated: 2014/03/27 18:50:55 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_defines.h"
#include "ft_shell_command.h"
#include "ft_shell_terminal.h"
#include "libft.h"
#include <stdint.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <unistd.h>

/******************************************************************************
 * FONCTIONS UTILES POUR LA COMMANDE
 ******************************************************************************/
static void ft_shell_command_delete(t_cmd *command);

t_cmd *ft_shell_command_new(const uint8_t *line,
                            t_cmd         *prev,
                            t_cmd         *next,
                            e_newcmdopt    option)
{
    t_cmd *cmd = (t_cmd *) malloc(sizeof(*cmd));

    switch (option)
    {
        case SHELL_COMMAND_NEW_STEAL_LINE:
        {
            cmd->origin = line;
            break;
        }
        case SHELL_COMMAND_NEW_DUPLICATE_LINE:
        {
            cmd->origin = ft_ustrdup(line);
            break;
        }
    }
    cmd->len  = ft_ustrlen(line);
    cmd->size = 0;
    while (cmd->size <= cmd->len)
    {
        cmd->size += SHELL_COMMAND_BUFFER_HOP;
    }
    cmd->buffer = (uint8_t *) ft_memalloc(cmd->size);
    ft_memcpy(cmd->buffer, line, cmd->len);
    cmd->pos    = cmd->len;
    cmd->option = 0;
    cmd->harea  = NULL;
    cmd->token  = NULL;
    cmd->prev   = prev;
    cmd->next   = next;
    if (prev != NULL)
    {
        prev->next = cmd;
    }
    if (next != NULL)
    {
        next->prev = cmd;
    }
    return (cmd);
}

t_cmd *ft_shell_command_get_first(const t_cmd *command_list)
{
    while (command_list != NULL)
    {
        if (command_list->next == NULL)
        {
            return ((t_cmd *) command_list);
        }
        command_list = command_list->next;
    }
    return ((t_cmd *) command_list);
}

t_cmd *ft_shell_command_get_last(const t_cmd *command_list)
{
    while (command_list != NULL)
    {
        if (command_list->prev == NULL)
        {
            return ((t_cmd *) command_list);
        }
        command_list = command_list->prev;
    }
    return ((t_cmd *) command_list);
}

void ft_command_change_pos(t_cmd  *command,
                           size_t  newval,
                           e_posop operation,
                           t_term *terminal)
{
    switch (operation)
    {
        case COMMAND_POS_ADD_VALUE:
        {
            command->pos += newval;
            break;
        }
        case COMMAND_POS_SUBSTRACT_VALUE:
        {
            command->pos -= newval;
            break;
        }
    }
    ft_shell_terminal_calc_current_command_position(terminal, command->pos);
    ft_term_move_cursor(terminal, MOVE_CURSOR_CURRENT);
}

void ft_shell_command_insert_character(t_cmd *command, uint8_t charac)
{
    size_t pos = 0;

    /* Réallocation de la taille du buffer de la commande avant insertion si nécessaire */
    if (command->len + 1 >= command->size)
    {
        ft_shell_command_realloc(command);
    }
    /* Copie des caractères d'un cran vers la droite à partir de la position courante */
    pos = command->len;
    while (pos > command->pos)
    {
        command->buffer[pos] = command->buffer[pos - 1];
        pos--;
    }
    /* Insertion du caractère dans le buffer */
    command->buffer[pos] = charac;
    command->pos += 1;
    command->len += 1;
    if (command->origin != NULL
        && !_test_bit(command->option, COMMAND_HISTORIC_MODIFIED))
    {
        _set_bit(command->option, COMMAND_HISTORIC_MODIFIED);
    }
}

void ft_shell_command_delete_character(t_cmd *command, e_cmdope operation, size_t size)
{
    uint8_t *buffer = command->buffer;
    size_t   len    = command->len;
    size_t   pos    = 0;

    if (operation == COMMAND_REMOVE_CHAR_LEFT)
    {
        command->pos -= size;
    }
    pos = command->pos;
    command->len -= size;
    ft_memcpy(buffer + pos, buffer + pos + size, len - pos);
    buffer[len] = '\0';
    /* Indique que le buffer d'origine a changé */
    if (command->origin != NULL
        && !_test_bit(command->option, COMMAND_HISTORIC_MODIFIED))
    {
        _set_bit(command->option, COMMAND_HISTORIC_MODIFIED);
    }
}

static t_higharea *find_closest_highlighted_area(t_higharea *harea, size_t pos)
{
    while (harea != NULL)
    {
        if (pos < harea->head)
        {
            break;
        }
        harea = harea->next;
    }
    return (harea);
}

void ft_shell_command_print(const t_cmd  *command,
                            const t_term *terminal,
                            e_printopt    option)
{
    const uint8_t *buffer = command->buffer;
    size_t         pos    = 0;
    t_higharea    *harea  = find_closest_highlighted_area(command->harea, pos);

    switch (option)
    {
        /* Imprime la commande depuis le début */
        case COMMAND_PRINT_FROM_START:
        {
            ft_term_move_cursor(terminal, MOVE_CURSOR_START);
            break;
        }
        /* Imprime la commande depuis la position */
        case COMMAND_PRINT_FROM_POS:
        {
            pos = command->pos;
            break;
        }
        /* Imprime la commande depuis la position - 1 */
        case COMMAND_PRINT_FROM_POS_LESS:
        {
            pos = command->pos - 1;
            break;
        }
    }
    /* Désactive le mode de surlignage */
    if (_test_bit(command->option, COMMAND_HIGHLIGHT_MODE))
    {
        ft_term_clear_modes(terminal);
    }
    /* Efface les caractères du terminal à partir du curseur */
    ft_term_clear_cursor_and_under(terminal);
    /* Réécriture de la commande */
    while (harea != NULL)
    {
        if (pos < harea->tail)
        {
            ft_term_clear_modes(terminal);
            write(STDOUT_FILENO, buffer + pos, harea->tail - pos);
            pos = harea->tail;
        }
        ft_term_highlight_mode_on(terminal);
        write(STDOUT_FILENO, buffer + pos, harea->head - pos);
        pos   = harea->head;
        harea = harea->next;
    }
    ft_term_clear_modes(terminal);
    write(STDOUT_FILENO, buffer + pos, command->len - pos);
    /* Positionne le curseur à la fin de la commande et
       efface les caractères du terminal à partir là */
    if (terminal->current.line != terminal->end.line)
    {
        ft_term_move_cursor(terminal, MOVE_CURSOR_END);
        ft_term_clear_cursor_and_under(terminal);
    }
    /* Repositionne le curseur à sa position d'origine */
    ft_term_move_cursor(terminal, MOVE_CURSOR_CURRENT);
    /* Réactive le mode de surlignage */
    if (_test_bit(command->option, COMMAND_HIGHLIGHT_MODE))
    {
        ft_term_highlight_mode_on(terminal);
    }
}

t_cmd *ft_shell_command_reinit(t_cmd *command)
{
    command = ft_shell_command_get_last(command);
    /* Réinitialisation des Commandes de l'historique */
    while (command->next != NULL)
    {
        if (_test_bit(command->option, COMMAND_HISTORIC_MODIFIED))
        {
            command->len = ft_ustrlen(command->origin);
            ft_memcpy(command->buffer, command->origin, command->len + 1);
        }
        command->option = 0;
        command->pos    = command->len;
        command->harea  = ft_command_highlight_remove_all(command->harea);
        command         = command->next;
    }
    /* Réinitialisation de la première Commande (qui ne fait pas partie de l'historique) */
    if (command->size != SHELL_COMMAND_BUFFER_HOP)
    {
        free((void *) command->buffer);
        command->buffer = (uint8_t *) malloc(SHELL_COMMAND_BUFFER_HOP);
        command->size   = SHELL_COMMAND_BUFFER_HOP;
    }
    ft_memset(command->buffer, 0, command->size);
    command->len    = 0;
    command->pos    = 0;
    command->option = 0;
    command->harea  = ft_command_highlight_remove_all(command->harea);
    return (command);
}

void ft_shell_command_realloc(t_cmd *command)
{
    uint8_t *old = command->buffer;

    command->buffer = (uint8_t *) ft_memalloc(command->size + SHELL_COMMAND_BUFFER_HOP);
    ft_memcpy(command->buffer, old, command->size);
    command->size += SHELL_COMMAND_BUFFER_HOP;
    free(old);
}

void ft_shell_command_delete_all(t_cmd *command)
{
    t_cmd *prev = NULL;

    command = ft_shell_command_get_first(command);
    while (command != NULL)
    {
        prev = command->prev;
        ft_shell_command_delete(command);
        command = prev;
    }
}

/**
 * @brief Supprime une commande.
 * @param command Commande à supprimer
 */
static void ft_shell_command_delete(t_cmd *command)
{
    free((void *) command->origin);
    free((void *) command->buffer);
    ft_command_highlight_remove_all(command->harea);
    ft_command_token_remove_all(command->token);
    free((void *) command->tokenbuf);
    free(command);
}

void ft_shell_command_remove_last(t_cmd **command)
{
    t_cmd *last = ft_shell_command_get_last(*command);

    if (last == NULL)
    {
        return;
    }
    if (last->next == NULL) /* Dernier élément de la liste */
    {
        *command = NULL;
    }
    else
    {
        last->next->prev = NULL;
    }
    ft_shell_command_delete(last);
}
