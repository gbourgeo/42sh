/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_signals.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/02/09 01:21:28 by gbourgeo          #+#    #+#             */
/*   Updated: 2014/03/30 22:00:35 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

extern t_shell  g_shell;  /* Structure de notre environnement global */

static void     ft_break_this_signal(int signum)
{
    if (signum == SIGINT)
        ft_putchar('\n');
    if (signum == SIGTSTP)
        main(0, NULL);
    if (signum == SIGWINCH)
    {
        ft_get_terminal_size(&g_shell);
        ft_get_cursor_position(&g_shell);
        debug((char [16]){0}, &g_shell);
    }
}

void            ft_signals(t_shell *shell)
{
    int     i;

    i = 1;
    while (i < _NSIG)
    {
        if (i != SIGKILL && i != SIGSTOP && i != 32 && i != 33 && i != 64)
        {
            shell->sigs[i - 1] = signal(i, &ft_break_this_signal);
            if (shell->sigs[i - 1] == SIG_ERR)
            {
                ft_error(shell, "Signal %d returned", i);
            }
        }
        else
        {
            shell->sigs[i - 1] = SIG_ERR;
        }
        i++;
    }
}
