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
#include "ft_minishell.h"

/**
 * Fonction de déplacement du curseur d'un caractère sur la droite.
 * Elle prend en compte le mode actif ou non de surlignage.
 * @param[in] shell_ctx Structure d'environnement du shell
*/
void        ft_move_cursor_right(void *shell_ctx)
{
    t_shell     *shell;
    char        *buffer;
    t_high_t    *text;
    size_t      pos;

    shell = (t_shell *)shell_ctx;
    buffer = shell->command.buffer;
    pos = shell->command.pos;
    if (buffer[pos] == '\0')
        return ;
    if (shell->highlighted.on)
    {
        text = shell->highlighted.texts;
        if (text->head < text->tail)
            ft_tputs("me", shell); /* Désactive tous les modes actifs */
        ft_tputs("im", shell); /* Active le mode insertion */
        ft_tputs("dc", shell); /* Efface 1 caractère sous la position du curseur */
        write(STDOUT_FILENO, buffer + pos, 1);
        ft_tputs("ei", shell); /* Désactive le mode insertion */
        if (text->head < text->tail)
            ft_tputs("mr", shell); /* Réactive le mode surlignage */
        text->head += 1;
    }
    shell->terminal.current_column += 1;
    if (shell->terminal.current_column > shell->terminal.max_column)
    {
        if (shell->terminal.current_line < shell->terminal.max_line)
            shell->terminal.current_line += 1;
        shell->terminal.current_column = 0;
        ft_tputs("nw", shell); /* Positionne le curseur au début de la ligne suivante */
        if (shell->command.buffer[shell->command.pos] == '\0')
            ft_tputs("ce", shell); /* Efface la ligne du curseur */
    }
    else
        ft_tgoto(shell->terminal.current_column, shell->terminal.current_line, shell);
    shell->command.pos += 1;
}

/**
 * Fonction de déplacement du curseur d'un caractère sur la gauche.
 * Elle prend en compte le mode actif ou non de surlignage.
 * @param[in] shell_ctx Structure d'environnement du shell
*/
void        ft_move_cursor_left(void *shell_ctx)
{
    t_shell     *shell;
    char        *buffer;
    t_high_t    *text;
    size_t      pos;

    shell = (t_shell *)shell_ctx;
    pos = shell->command.pos;
    if (pos <= 0)
        return ;
    if (shell->highlighted.on)
    {
        text = shell->highlighted.texts;
        if (text->head >= text->tail)
            ft_tputs("me", shell); /* Désactive tous les modes actifs */
        buffer = shell->command.buffer;
        if (buffer[pos] != '\0')
        {
            ft_tputs("im", shell); /* Active le mode insertion */
            ft_tputs("dc", shell); /* Efface 1 caractère sous la position du curseur */
            write(STDOUT_FILENO, buffer + pos, 1);
            ft_tputs("le", shell); /* Bouge le curseur à gauche d'1 colonne */
            ft_tputs("ei", shell); /* Désactive le mode insertion */
        }
        if (text->head >= text->tail)
            ft_tputs("mr", shell); /* Réactive le mode surlignage */
        text->head -= 1;
    }
    shell->terminal.current_column -= 1;
    if (shell->terminal.current_column < 0)
    {
        if (shell->terminal.current_line > 0)
            shell->terminal.current_line -= 1;
        shell->terminal.current_column = shell->terminal.max_column;
    }
    ft_tgoto(shell->terminal.current_column, shell->terminal.current_line, shell);
    shell->command.pos -= 1;
}

/**
 * Fonction de déplacement du curseur à la fin de la commande.
 * Elle prend en compte le mode actif ou non de surlignage.
 * @param[in] shell_ctx Structure d'environnement du shell
*/
void        ft_move_cursor_end_of_command(void *shell_ctx)
{
    t_shell *shell;
    size_t  buffer_len;

    shell = (t_shell *)shell_ctx;
    buffer_len = ft_strlen(shell->command.buffer);
    while (shell->command.pos < buffer_len)
        ft_move_cursor_right(shell);
}

/**
 * Fonction de déplacement du curseur au début de la commande.
 * Elle prend en compte le mode actif ou non de surlignage.
 * @param[in] shell_ctx Structure d'environnement du shell
*/
void        ft_move_cursor_start_of_commmand(void *shell_ctx)
{
    t_shell *shell;

    shell = (t_shell *)shell_ctx;
    while (shell->command.pos > 0)
        ft_move_cursor_left(shell);
}

/**
 * Fonction de déplacement du curseur d'un mot sur la droite.
 * Elle prend en compte le mode actif ou non de surlignage.
 * @param[in] shell_ctx Structure d'environnement du shell
*/
void        ft_move_word_right(void *shell_ctx)
{
    t_shell *shell;
    char    *buffer;
    size_t  buffer_len;

    shell = (t_shell *)shell_ctx;
    buffer = shell->command.buffer;
    buffer_len = ft_strlen(shell->command.buffer);
    while (shell->command.pos < buffer_len
    && ft_iswhitespace(buffer[shell->command.pos]))
    {
        ft_move_cursor_right(shell);
    }
    while (shell->command.pos < buffer_len
    && ft_isprint(buffer[shell->command.pos])
    && !ft_iswhitespace(buffer[shell->command.pos]))
    {
        ft_move_cursor_right(shell);
    }
}


/**
 * Fonction de déplacement du curseur d'un mot sur la gauche.
 * Elle prend en compte le mode actif ou non de surlignage.
 * @param[in] shell_ctx Structure d'environnement du shell
*/
void        ft_move_word_left(void *shell_ctx)
{
    t_shell *shell;
    char    *buffer;

    shell = (t_shell *)shell_ctx;
    buffer = shell->command.buffer;
    while (shell->command.pos > 0
    && ft_iswhitespace(buffer[shell->command.pos - 1]))
    {
        ft_move_cursor_left(shell);
    }
    while (shell->command.pos > 0
    && ft_isprint(buffer[shell->command.pos - 1])
    && !ft_iswhitespace(buffer[shell->command.pos - 1]))
    {
        ft_move_cursor_left(shell);
    }
}

void        ft_move_cursor_up(void *shell_ctx)
{
    t_shell *shell;
    int     i;

    shell = (t_shell *)shell_ctx;
    i = 0;
    while (i++ <= shell->terminal.max_column)
        ft_move_cursor_left(shell);
}

void        ft_move_cursor_down(void *shell_ctx)
{
    t_shell *shell;
    int     i;

    shell = (t_shell *)shell_ctx;
    i = 0;
    while (i++ <= shell->terminal.max_column)
        ft_move_cursor_right(shell);
}
