/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_key_esc.c                                       :+:      :+:    :+:   */
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

void ft_clear_modes(t_shell *shell)
{
    int print = 0;

    if (shell->command->harea != NULL)
    {
        shell->command->harea = ft_command_highlight_remove_all(shell->command->harea); /* Supprime les zones de texte surligné de la commande */
        print                 = 1;
    }
    if (TEST_BIT(shell->command->option, COMMAND_HIGHLIGHT_MODE))
    {
        REMOVE_BIT(shell->command->option, COMMAND_HIGHLIGHT_MODE); /* Désactive le surlignage */
        ft_term_clear_modes(&shell->terminal);                      /* Désactive tous les modes actifs */
        print = 1;
    }
    if (print == 1)
    {
        /* Réaffichage de la commande */
        ft_shell_command_print(shell->command, &shell->terminal, COMMAND_PRINT_FROM_START);
    }
}
