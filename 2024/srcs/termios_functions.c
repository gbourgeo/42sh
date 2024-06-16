/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termios_functions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/02/09 12:00:56 by gbourgeo          #+#    #+#             */
/*   Updated: 2014/03/27 19:09:42 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "ft_minishell.h"

static int  ft_pchar(int nb)
{
    return (write(STDIN_FILENO, &nb, sizeof(nb)));
}

void        ft_tputs(const char *t, t_shell *shell)
{
    char    *tt;

    tt = tgetstr(t, NULL);
    if (tt == NULL)
        ft_error(shell, "\"%s\" not available on this terminal", t);
    else
        tputs(tt, 1, ft_pchar);
}

void        ft_tgoto(int column, int line, t_shell *shell)
{
    char    *tt;

    tt = tgetstr("cm", NULL);
    if (tt == NULL)
        ft_error(shell, "\"cm\" not available on this terminal");
    else
        tputs(tgoto(tt, column, line), 1, ft_pchar);
}

void        ft_load_termcaps(t_shell *shell)
{
    struct termios  termios;

    termios = shell->terminal.ios;
    termios.c_lflag &= ~(ICANON | ECHO | ISIG);
    termios.c_cc[VMIN] = 1;
    termios.c_cc[VTIME] = 0;
    if ((tcsetattr(shell->fd, TCSANOW, &termios)) == -1)
        ft_error_fatal(shell, "Can't get terminal attributes");
}

void        ft_restore_termcaps(t_shell *shell)
{
    if (shell->options & SHELL_TERMCAPS_LOADED
    &&  tcsetattr(shell->fd, TCSANOW, &shell->terminal.ios) == -1)
        ft_error_fatal(shell, "Can't reset terminal attributes");
}
