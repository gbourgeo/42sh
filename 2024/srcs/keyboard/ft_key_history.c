/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_key_history.c                                   :+:      :+:    :+:   */
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
#include <stddef.h>
#include <stdlib.h>

/**
 * @brief Fonction d'affichage de la commande précédente.
 * @param[in] shell_ctx Environnement du shell
 */
void ft_print_prev_command(t_shell *shell)
{
    /* Pas de commande précédente */
    if (shell->command->next == NULL)
    {
        return;
    }
    shell->command = shell->command->next;
    ft_shell_terminal_calc_end_command_position(&shell->terminal, shell->command->len);
    ft_shell_command_print(shell->command,
                           &shell->terminal,
                           UINT32(COMMAND_PRINT_FROM_START)
                           | UINT32(COMMAND_PRINT_SET_CURSOR_END));
}

/**
 * @brief Fonction d'affichage de la commande suivante.
 * @param[in] shell_ctx Environnement du shell
 */
void ft_print_next_command(t_shell *shell)
{
    /* Pas de commande suivante */
    if (shell->command->prev == NULL)
    {
        return;
    }
    shell->command = shell->command->prev;
    ft_shell_terminal_calc_end_command_position(&shell->terminal, shell->command->len);
    ft_shell_command_print(shell->command,
                           &shell->terminal,
                           UINT32(COMMAND_PRINT_FROM_START)
                           | UINT32(COMMAND_PRINT_SET_CURSOR_END));
}
