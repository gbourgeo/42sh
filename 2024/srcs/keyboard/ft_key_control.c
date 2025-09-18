/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_key_control.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/03/13 15:58:56 by gbourgeo          #+#    #+#             */
/*   Updated: 2018/04/06 18:38:49 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_defines.h"
#include "ft_shell.h"
#include "ft_shell_builtins.h"
#include "ft_shell_command.h"
#include "ft_shell_terminal.h"
#include "ft_shell_termkeys.h"
#include <unistd.h>

void ft_control_c(t_shell *shell)
{
    /* Reset des modes */
    if (TEST_BIT(shell->command->option, COMMAND_HIGHLIGHT_MODE))
    {
        ft_term_clear_modes(&shell->terminal);
        REMOVE_BIT(shell->command->option, COMMAND_HIGHLIGHT_MODE);
    }
    write(STDOUT_FILENO, "^C", 2);
    ft_move_cursor_end_of_command(shell);
    ft_term_move_cursor_down(&shell->terminal);
    /* Reset de la commande */
    shell->command = ft_shell_command_reinit(shell->command);
    shell->prompt.print = 1;
}

void ft_control_d(t_shell *shell)
{
    if (shell->command->len == 0)
    {
        ft_exit(NULL, shell);
    }
}
