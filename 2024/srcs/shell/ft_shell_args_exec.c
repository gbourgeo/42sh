/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_shell_loop_args.c                               :+:      :+:    :+:   */
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
#include "ft_shell_prompt.h"
#include "ft_shell_terminal.h"
#include <errno.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <sys/fcntl.h>
#include <sys/signal.h>
#include <unistd.h>

/**
 * @brief Fonction générale de capture des signaux.
 * @param signum Numéro du signal capturé
 */
static void ft_break_this_signal(int signum)
{
    if (signum == SIGINT)
    {
        write(STDOUT_FILENO, "\n", 1);
    }
    // if (signum == SIGTSTP)
    //     main(0, NULL);
    else if (signum == SIGWINCH)
    {
        extern t_shell g_shell;

        ft_shell_terminal_get_size(&g_shell.terminal);
        ft_shell_terminal_get_cursor_position(&g_shell.terminal, SET_CURSOR_CURRENT);
        ft_shell_command_debug((const uint8_t[SHELL_KEY_SIZE]) { 0 }, SHELL_KEY_SIZE);
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
                ft_shell_log(SH_LOG_LEVEL_FATAL, "signal %d: %s", iter, strerror(errno));
                errno = 0;
            }
        }
        else
        {
            shell->sigs[iter - 1] = SIG_ERR;
        }
        iter++;
    }
}

void ft_shell_args_exec(const char **argv, t_shell *shell)
{
    if (argv != NULL && *argv != NULL)
    {
        shell->fd = open(argv[0], O_RDONLY | O_CLOEXEC);
        if (shell->fd == -1)
        {
            ft_shell_log(SH_LOG_LEVEL_FATAL, "%s: %s", strerror(errno), argv[0]);
            errno = 0;
        }
        else
        {
            ft_shell_log(SH_LOG_LEVEL_DBG, "File '%s' opened successfully", argv[0]);
        }
    }
    else
    {
        ASSIGN_BIT(shell->options, SHELL_INTERACTIVE_MODE);
        shell->fd = ft_shell_terminal_load(&shell->terminal, ft_shell_env_get_value("TERM", &shell->environ));
        if (shell->fd > 0)
        {
            ASSIGN_BIT(shell->options, SHELL_TERMATTR_LOADED);
            // TODO(gbo): Parsing du fichier de conf du shell type .42shrc
            shell->command_size = ft_shell_history_parse_file(&shell->command,
                                                              &shell->history,
                                                              &shell->environ,
                                                              shell->progname);
            shell->command      = ft_shell_command_new(NULL, shell->command, NULL, 0);
            ft_shell_prompt_create(&shell->prompt, &shell->environ);
            ft_catch_signals(shell);
        }
    }
}
