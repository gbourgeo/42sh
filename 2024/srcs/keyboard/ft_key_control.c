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

#include "ft_shell.h"
#include "ft_shell_builtins.h"
#include "ft_shell_command.h"
#include "ft_shell_terminal.h"
#include "ft_shell_termkeys.h"
#include <unistd.h>

void ft_control_c(t_shell *shell)
{
    write(STDOUT_FILENO, "^C", 2);
    ft_shell_interactive_reinit(shell, SHELL_COMMAND_DONT_SAVE);
}

void ft_control_d(t_shell *shell)
{
    if (shell->command->len == 0)
    {
        ft_exit(NULL, shell);
    }
}
