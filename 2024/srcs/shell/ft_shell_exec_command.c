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
#include "ft_shell_history.h"
#include "ft_shell_prompt.h"
#include "ft_shell_terminal.h"

void ft_shell_exec_command(t_shell *shell)
{
    const char *ifs = ft_shell_env_get_value("IFS", &shell->environ);
    if (ft_shell_command_parse(shell->command, ifs, shell->options, &shell->terminal) == SHELL_COMMAND_PARSED)
    {
        ft_shell_interactive_reinit(shell, SHELL_COMMAND_SAVE);
    }
}

void ft_shell_interactive_reinit(t_shell *shell, e_shcmdsave savecmd)
{
    if (_test_bit(shell->options, SHELL_INTERACTIVE_MODE))
    {
        if (savecmd == SHELL_COMMAND_SAVE)
        {
            ft_shell_history_save_command(&shell->history, shell->command);
        }
        /* Désactive tous les modes actifs */
        if (_test_bit(shell->command->option, COMMAND_HIGHLIGHT_MODE))
        {
            _remove_bit(shell->command->option, COMMAND_HIGHLIGHT_MODE);
            ft_term_clear_modes(&shell->terminal);
        }
        /* Supprime les zones de texte surligné */
        shell->command->harea = ft_command_highlight_remove_all(shell->command->harea);
        /* Deplace le curseur en fin de commande */
        ft_term_move_cursor(&shell->terminal, MOVE_CURSOR_END);
        /* Déplace le curseur au début de la ligne suivante */
        ft_term_move_cursor_down(&shell->terminal);
        /* Efface la ligne et celles sous le curseur */
        ft_term_clear_line_and_under(&shell->terminal);
        /* Reconstruction du prompt */
        ft_shell_prompt_create(&shell->prompt, &shell->environ);
        shell->command = ft_shell_command_reinit(shell->command);
    }
}
