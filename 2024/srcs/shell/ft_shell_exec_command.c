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
#include "ft_shell_command.h"
#include "ft_shell_constants.h"
#include "ft_shell_environ.h"
#include "ft_shell_terminal.h"
#include "ft_shell_termkeys.h"

static void ft_shell_interactive_reinit(t_shell *shell);

void ft_shell_exec_command(t_shell *shell)
{
    if (shell->command->len != 0)
    {
        const char *ifs = ft_shell_env_get_value("IFS", &shell->environ);
        ft_term_move_cursor(&shell->terminal, MOVE_CURSOR_END);
        ft_term_move_cursor_down(&shell->terminal);
        ft_term_clear_line_and_under(&shell->terminal);
        if (ft_shell_command_parse(shell->command, ifs) == SHELL_COMMAND_PARSED)
        {
            ft_shell_interactive_reinit(shell);
        }
    }
}

/**
 * @brief Sauvegarde la Commande, réinitialise les Zones de textes surlignés,
 * demande de réaffichage du prompt, réinitialise les modes du terminal,
 * réinitialise la Commande.
 * @param shell Structure du Shell
 */
static void ft_shell_interactive_reinit(t_shell *shell)
{
    if (TEST_BIT(shell->options, SHELL_INTERACTIVE_MODE))
    {
        shell->command_size   = ft_shell_command_save(shell->command,
                                                    shell->command_size,
                                                    shell->history.max_size);
        /* Supprime les zones de texte surligné */
        shell->command->harea = ft_command_highlight_remove_all(shell->command->harea);
        /* Deplace le curseur en fin de commande */
        ft_move_cursor_end_of_command(shell);
        /* Déplace le curseur au début de la ligne suivante */
        ft_term_move_cursor_down(&shell->terminal);
        /* Efface la ligne et celles sous le curseur */
        ft_term_clear_line_and_under(&shell->terminal);
        /* Demande de réaffichage du prompt */
        shell->prompt.print = 1;
        /* Désactive tous les modes actifs */
        if (TEST_BIT(shell->command->option, COMMAND_HIGHLIGHT_MODE))
        {
            REMOVE_BIT(shell->command->option, COMMAND_HIGHLIGHT_MODE);
            ft_term_clear_modes(&shell->terminal);
        }
        shell->command = ft_shell_command_reinit(shell->command);
    }
}
