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

#include "ft_command.h"
#include "ft_highlight.h"
#include "ft_history.h"
#include "ft_shell.h"
#include "ft_termios.h"
#include "libft.h"

void ft_shell_exit(t_shell *shell)
{
    int i;

    i = 1;
    ft_clear_shell_terminal(&shell->terminal, shell->options & SHELL_TERMATTR_LOADED);
    ft_freestr((char **) &shell->bin_path);
    ft_freetab((char ***) &shell->global_env);
    ft_freetab((char ***) &shell->internal_env);
    ft_command_clear(&shell->command);
    ft_history_remove_all(shell->history);
    ft_highlight_remove_all(&shell->highlighted.texts);
    ft_freestr(&shell->highlighted.yank);
    while (i < NSIG)
    {
        if (shell->sigs[i - 1] != SIG_ERR)
            signal(i, shell->sigs[i - 1]);
        i++;
    }
}
