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

#include "ft_shell.h"
#include "ft_termios.h"
#include "libft.h"

void ft_move_shift_right(t_shell *shell)
{
    if (shell->command.buffer[shell->command.pos] != '\0')
    {
        if (shell->highlighted.on == 0)
        {
            shell->highlighted.on   = 1;
            shell->highlighted.tail = shell->command.pos;
            ft_term_highlight_mode_on(&shell->terminal); /* Active le mode surlignage */
            ft_term_remove_char(&shell->terminal);       /* Efface 1 caractère sous la position du curseur */
            ft_putchar(shell->command.buffer[shell->command.pos]);
            ft_term_clear_modes(&shell->terminal);       /* Désactive tous les modes actifs */
            shell->command.pos++;
            return;
        }
        if (shell->command.pos >= shell->highlighted.tail)
        {
            ft_term_highlight_mode_on(&shell->terminal); /* Active le mode surlignage */
        }
        ft_term_remove_char(&shell->terminal);           /* Efface 1 caractère sous la position du curseur */
        ft_putchar(shell->command.buffer[shell->command.pos]);
        if (shell->command.pos >= shell->highlighted.tail)
        {
            ft_term_clear_modes(&shell->terminal); /* Désactive tous les modes actifs */
        }
        shell->command.pos++;
    }
}

void ft_move_shift_left(t_shell *shell)
{
    if (shell->command.pos > 0)
    {
        if (shell->highlighted.on == 0)
        {
            shell->highlighted.on   = 1;
            shell->highlighted.tail = shell->command.pos;
            if (shell->command.buffer[shell->command.pos] != '\0')
            {
                ft_term_highlight_mode_on(&shell->terminal); /* Active le mode surlignage */
                ft_term_remove_char(&shell->terminal);       /* Efface 1 caractère sous la position du curseur */
                ft_putchar(shell->command.buffer[shell->command.pos]);
                ft_term_move_cursor_left(&shell->terminal);  /* Bouge le curseur à gauche d'1 colonne */
            }
            ft_term_move_cursor_left(&shell->terminal);      /* Bouge le curseur à gauche d'1 colonne */
            if (shell->command.buffer[shell->command.pos] != '\0')
            {
                ft_term_clear_modes(&shell->terminal); /* Désactive tous les modes actifs */
            }
            shell->command.pos--;
            return;
        }
        if (shell->command.pos <= shell->highlighted.tail)
        {
            ft_term_highlight_mode_on(&shell->terminal); /* Active le mode surlignage */
        }
        if (shell->command.buffer[shell->command.pos] != '\0')
        {
            ft_term_remove_char(&shell->terminal);      /* Efface 1 caractère sous la position du curseur */
            ft_putchar(shell->command.buffer[shell->command.pos]);
            ft_term_move_cursor_left(&shell->terminal); /* Bouge le curseur à gauche d'1 colonne */
        }
        ft_term_move_cursor_left(&shell->terminal);     /* Bouge le curseur à gauche d'1 colonne */
        if (shell->command.pos <= shell->highlighted.tail)
        {
            ft_term_clear_modes(&shell->terminal); /* Désactive tous les modes actifs */
        }
        shell->command.pos--;
    }
}
