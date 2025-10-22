/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_shell_exit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/02/09 03:00:47 by gbourgeo          #+#    #+#             */
/*   Updated: 2014/03/27 18:39:53 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_defines.h"
#include "ft_shell.h"
#include "ft_shell_command.h"
#include "ft_shell_constants.h"
#include "ft_shell_environ.h"
#include "ft_shell_history.h"
#include "ft_shell_log.h"
#include "ft_shell_terminal.h"
#include <signal.h>
#include <stdlib.h>
#include <termios.h>

void ft_shell_exit(t_shell *shell)
{
    int iter = 1;

    if (_test_bit(shell->options, SHELL_TERMATTR_LOADED)
     && tcsetattr(shell->fd, TCSANOW, &shell->terminal.ios) == -1)
    {
        ft_shell_log(SH_LOG_LEVEL_FATAL, "failed to restore terminal attributes");
    }
    ft_shell_history_save_to_file(&shell->history, shell->command);
    ft_shell_command_delete_all(shell->command);
    free(shell->yank);
    ft_shell_env_free(&shell->environ);
    while (iter < NSIG)
    {
        if (shell->sigs[iter - 1] != SIG_ERR)
        {
            (void) signal(iter, shell->sigs[iter - 1]);
        }
        iter++;
    }
}
