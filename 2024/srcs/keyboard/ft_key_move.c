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

#include "ft_termkeys.h"
#include "libft.h"

#include <unistd.h>

/**
 * Fonction de déplacement du curseur d'un caractère sur la droite.
 * Elle prend en compte le mode actif ou non de surlignage.
 * @param[in] shell_ctx Structure d'environnement du shell
 */
void ft_move_cursor_right(t_shell *shell)
{
    char    *buffer;
    t_htext *text;
    size_t   pos;

    buffer = shell->command.buffer;
    pos    = shell->command.pos;
    if (buffer[pos] == '\0')
        return;
    if (shell->highlighted.on)
    {
        text = shell->highlighted.texts;
        if (text->head < text->tail)
            ft_term_clear_modes(&shell->terminal);       /* Désactive tous les modes actifs */
        ft_term_insert_mode_on(&shell->terminal);        /* Active le mode insertion */
        ft_term_remove_char(&shell->terminal);           /* Efface 1 caractère sous la position du curseur */
        write(STDOUT_FILENO, buffer + pos, 1);
        ft_term_insert_mode_off(&shell->terminal);       /* Désactive le mode insertion */
        if (text->head < text->tail)
            ft_term_highlight_mode_on(&shell->terminal); /* Réactive le mode surlignage */
        text->head += 1;
    }
    shell->terminal.current_column += 1;
    if (shell->terminal.current_column > shell->terminal.max_column)
    {
        if (shell->terminal.current_line < shell->terminal.max_line)
            shell->terminal.current_line += 1;
        shell->terminal.current_column = 0;
        ft_term_move_cursor_down(&shell->terminal);           /* Positionne le curseur au début de la ligne suivante */
        if (shell->command.buffer[shell->command.pos] == '\0')
            ft_term_clear_cursor_and_under(&shell->terminal); /* Efface la ligne du curseur */
    }
    else
        ft_term_move_cursor(&shell->terminal, shell->terminal.current_column, shell->terminal.current_line);
    shell->command.pos += 1;
}

/**
 * Fonction de déplacement du curseur d'un caractère sur la gauche.
 * Elle prend en compte le mode actif ou non de surlignage.
 * @param[in] shell_ctx Structure d'environnement du shell
 */
void ft_move_cursor_left(t_shell *shell)
{
    char    *buffer;
    t_htext *text;
    size_t   pos;

    pos = shell->command.pos;
    if (pos <= 0)
        return;
    if (shell->highlighted.on)
    {
        text = shell->highlighted.texts;
        if (text->head >= text->tail)
            ft_term_clear_modes(&shell->terminal); /* Désactive tous les modes actifs */
        buffer = shell->command.buffer;
        if (buffer[pos] != '\0')
        {
            ft_term_insert_mode_on(&shell->terminal);   /* Active le mode insertion */
            ft_term_remove_char(&shell->terminal);      /* Efface 1 caractère sous la position du curseur */
            write(STDOUT_FILENO, buffer + pos, 1);
            ft_term_move_cursor_left(&shell->terminal); /* Bouge le curseur à gauche d'1 colonne */
            ft_term_insert_mode_off(&shell->terminal);  /* Désactive le mode insertion */
        }
        if (text->head >= text->tail)
            ft_term_highlight_mode_on(&shell->terminal); /* Réactive le mode surlignage */
        text->head -= 1;
    }
    shell->terminal.current_column -= 1;
    if (shell->terminal.current_column < 0)
    {
        if (shell->terminal.current_line > 0)
            shell->terminal.current_line -= 1;
        shell->terminal.current_column = shell->terminal.max_column;
    }
    ft_term_move_cursor(&shell->terminal, shell->terminal.current_column, shell->terminal.current_line);
    shell->command.pos -= 1;
}

/**
 * Fonction de déplacement du curseur à la fin de la commande.
 * Elle prend en compte le mode actif ou non de surlignage.
 * @param[in] shell_ctx Structure d'environnement du shell
 */
void ft_move_cursor_end_of_command(t_shell *shell)
{
    size_t buffer_len;

    buffer_len = ft_strlen(shell->command.buffer);
    while (shell->command.pos < buffer_len)
        ft_move_cursor_right(shell);
}

/**
 * Fonction de déplacement du curseur au début de la commande.
 * Elle prend en compte le mode actif ou non de surlignage.
 * @param[in] shell_ctx Structure d'environnement du shell
 */
void ft_move_cursor_start_of_commmand(t_shell *shell)
{
    while (shell->command.pos > 0)
        ft_move_cursor_left(shell);
}

/**
 * Fonction de déplacement du curseur d'un mot sur la droite.
 * Elle prend en compte le mode actif ou non de surlignage.
 * @param[in] shell_ctx Structure d'environnement du shell
 */
void ft_move_word_right(t_shell *shell)
{
    char  *buffer;
    size_t buffer_len;

    buffer     = shell->command.buffer;
    buffer_len = ft_strlen(shell->command.buffer);
    while (shell->command.pos < buffer_len
           && ft_iswhitespace(buffer[shell->command.pos]))
        ft_move_cursor_right(shell);
    while (shell->command.pos < buffer_len
           && ft_isprint(buffer[shell->command.pos])
           && !ft_iswhitespace(buffer[shell->command.pos]))
        ft_move_cursor_right(shell);
}

/**
 * Fonction de déplacement du curseur d'un mot sur la gauche.
 * Elle prend en compte le mode actif ou non de surlignage.
 * @param[in] shell_ctx Structure d'environnement du shell
 */
void ft_move_word_left(t_shell *shell)
{
    char *buffer;

    buffer = shell->command.buffer;
    while (shell->command.pos > 0
           && ft_iswhitespace(buffer[shell->command.pos - 1]))
        ft_move_cursor_left(shell);
    while (shell->command.pos > 0
           && ft_isprint(buffer[shell->command.pos - 1])
           && !ft_iswhitespace(buffer[shell->command.pos - 1]))
        ft_move_cursor_left(shell);
}

void ft_move_cursor_up(t_shell *shell)
{
    int i;

    i = 0;
    while (i++ <= shell->terminal.max_column)
        ft_move_cursor_left(shell);
}

void ft_move_cursor_down(t_shell *shell)
{
    int i;

    i = 0;
    while (i++ <= shell->terminal.max_column)
        ft_move_cursor_right(shell);
}
