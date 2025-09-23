/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_shell_exec_command.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/02/12 22:32:55 by gbourgeo          #+#    #+#             */
/*   Updated: 2014/03/27 18:50:55 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_defines.h"
#include "ft_shell.h"
#include "ft_shell_builtins.h"
#include "ft_shell_command.h"
#include "ft_shell_constants.h"
#include "ft_shell_terminal.h"
#include "ft_shell_termkeys.h"

void ft_shell_exec_command(t_shell *shell)
{
    if (TEST_BIT(shell->options, SHELL_INTERACTIVE_MODE))
    {
        if (TEST_BIT(shell->command->option, COMMAND_HIGHLIGHT_MODE))
        {
            REMOVE_BIT(shell->command->option, COMMAND_HIGHLIGHT_MODE);
            ft_term_clear_modes(&shell->terminal);                              /* Désactive tous les modes actifs */
        }
        shell->command->harea = ft_highlight_remove_all(shell->command->harea); /* Supprime les zones de texte surligné de la commande */
        ft_move_cursor_end_of_command(shell);                                   /* Deplace le curseur en fin de commande */
        ft_term_move_cursor_down(&shell->terminal);                             /* Déplace le curseur au début de la ligne suivante */
        ft_term_clear_line_and_under(&shell->terminal);                         /* Efface la ligne et celles sous le curseur */
        shell->prompt.print = 1;
    }
    if (shell->command->len != 0)
    {
        ft_shell_command_parse(shell->command, ft_getenv("IFS", shell));
        if (TEST_BIT(shell->options, SHELL_INTERACTIVE_MODE))
        {
            shell->command_size = ft_shell_command_save(&shell->command,
                                                        shell->command_size,
                                                        shell->history.max_size,
                                                        SHELL_COMMAND_SAVE_FROM_INTERACTIVE_MODE);
        }
    }
    shell->command = ft_shell_command_reinit(shell->command);
}
