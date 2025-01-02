/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_shell_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/02/09 03:00:47 by gbourgeo          #+#    #+#             */
/*   Updated: 2014/03/27 18:39:53 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_log.h"
#include "ft_shell.h"
#include "ft_termios.h"
#include "ft_termkeys.h"
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <sys/fcntl.h>
#include <unistd.h>

extern t_shell g_shell; /* Structure de notre environnement global */

/**
 * @brief Fonction générale de la capture des signaux.
 * @param signum Numéro du signal capturé
 */
static void    ft_break_this_signal(int signum)
{
    if (signum == SIGINT)
    {
        write(STDOUT_FILENO, "\n", 1);
    }
    // if (signum == SIGTSTP)
    //     main(0, NULL);
    else if (signum == SIGWINCH)
    {
        ft_get_terminal_size(&g_shell.terminal);
        ft_get_cursor_position(&g_shell.terminal);
        // debug_command_line((char [16]){0}, &g_shell);
    }
}

/**
 * @brief Capture des signaux: SIGINT, SIGTSTP, SIGWINCH
 * @param shell Structure interne du shell
 */
static void ft_catch_signals(t_shell *shell)
{
    int iter = 1;

#pragma unroll NSIG
    while (iter < NSIG)
    {
        if (iter == SIGINT || iter == SIGTSTP || iter == SIGWINCH)
        {
            shell->sigs[iter - 1] = signal(iter, &ft_break_this_signal);
            if (shell->sigs[iter - 1] == SIG_ERR)
            {
                ft_log(SH_LOG_LEVEL_FATAL, "signal: catching %d failed", iter);
            }
        }
        else
        {
            shell->sigs[iter - 1] = SIG_ERR;
        }
        iter++;
    }
}

void ft_shell_exec(const char **argv, t_shell *shell)
{
    char buf[MAX_KEY_SIZE] = { 0 };
    long ret               = 1;

    if (argv && *argv)
    {
        shell->terminal.fd = open(argv[0], O_RDONLY | O_CLOEXEC);
        if (shell->terminal.fd == -1)
        {
            ft_log(SH_LOG_LEVEL_FATAL, "Unable to open file: %s", argv[0]);
        }
    }
    else
    {
        shell->options |= (unsigned int) SHELL_INTERACTIVE_MODE;
        ft_load_termcaps(&shell->terminal, shell);
        if (ft_change_terminal_attributes(&shell->terminal) == 0)
        {
            shell->options |= (unsigned int) SHELL_TERMATTR_LOADED;
        }
        ft_catch_signals(shell);
        shell->prompt.print = 1;
    }
    /* Loop */
    while (shell->quit == 0 && ret > 0)
    {
        if (shell->prompt.print != 0)
        {
            ft_shell_prompt(shell);
            debug_command_line(buf, ret, shell);
        }
        ret = read(shell->terminal.fd, buf, sizeof(buf));
        if (ret < 0)
        {
            ft_log(SH_LOG_LEVEL_FATAL, "read: %s", strerror(errno));
        }
        ft_key_analyser(buf, ret, shell);
    }
}
