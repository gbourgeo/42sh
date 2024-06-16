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

void        ft_move_shift_right(void *e)
{
    t_shell *shell;

    shell = (t_shell *)e;
    if (shell->command.buffer[shell->command.pos] != '\0')
    {
        if (shell->highlighted.on == 0)
        {
            shell->highlighted.on = 1;
            shell->highlighted.tail = shell->command.pos;
            ft_tputs("mr", shell); /* Active le mode surlignage */
            ft_tputs("dc", shell); /* Efface 1 caractère sous la position du curseur */
            ft_putchar(shell->command.buffer[shell->command.pos]);
            ft_tputs("me", shell); /* Désactive tous les modes actifs */
            shell->command.pos++;
            return ;
        }
        if (shell->command.pos >= shell->highlighted.tail)
            ft_tputs("mr", shell); /* Active le mode surlignage */
        ft_tputs("dc", shell); /* Efface 1 caractère sous la position du curseur */
        ft_putchar(shell->command.buffer[shell->command.pos]);
        if (shell->command.pos >= shell->highlighted.tail)
            ft_tputs("me", shell); /* Désactive tous les modes actifs */
        shell->command.pos++;
    }
}

void        ft_move_shift_left(void *e)
{
    t_shell *shell;

    shell = (t_shell *)e;
    if (shell->command.pos > 0)
    {
        if (shell->highlighted.on == 0)
        {
            shell->highlighted.on = 1;
            shell->highlighted.tail = shell->command.pos;
            if (shell->command.buffer[shell->command.pos] != '\0')
            {
                ft_tputs("mr", shell); /* Active le mode surlignage */
                ft_tputs("dc", shell); /* Efface 1 caractère sous la position du curseur */
                ft_putchar(shell->command.buffer[shell->command.pos]);
                ft_tputs("le", shell); /* Bouge le curseur à gauche d'1 colonne */
            }
            ft_tputs("le", shell); /* Bouge le curseur à gauche d'1 colonne */
            if (shell->command.buffer[shell->command.pos] != '\0')
                ft_tputs("me", shell); /* Désactive tous les modes actifs */
            shell->command.pos--;
            return ;
        }
        if (shell->command.pos <= shell->highlighted.tail)
            ft_tputs("mr", shell); /* Active le mode surlignage */
        if (shell->command.buffer[shell->command.pos] != '\0')
        {
            ft_tputs("dc", shell); /* Efface 1 caractère sous la position du curseur */
            ft_putchar(shell->command.buffer[shell->command.pos]);
            ft_tputs("le", shell); /* Bouge le curseur à gauche d'1 colonne */
        }
        ft_tputs("le", shell); /* Bouge le curseur à gauche d'1 colonne */
        if (shell->command.pos <= shell->highlighted.tail)
            ft_tputs("me", shell); /* Désactive tous les modes actifs */
        shell->command.pos--;
    }
}
