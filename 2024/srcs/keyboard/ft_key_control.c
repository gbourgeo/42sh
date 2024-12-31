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

#include "ft_builtins.h"
#include "ft_command.h"
#include "ft_highlight.h"
#include "ft_shell.h"
#include "ft_termios.h"
#include "ft_termkeys.h"

#include <unistd.h>

void ft_control_c(t_shell *shell)
{
    ft_term_clear_modes(&shell->terminal);
    shell->highlighted.on = 0;
    ft_highlight_remove_all(&shell->highlighted.texts);
    write(STDOUT_FILENO, "^C", 2);
    ft_move_cursor_end_of_command(shell);
    ft_term_move_cursor_down(&shell->terminal);
    ft_command_reinit(&shell->command);
    shell->prompt.print = 1;
}

void ft_control_d(t_shell *shell)
{
    if (shell->command.buffer_len == 0)
        ft_exit(NULL, shell);
}
