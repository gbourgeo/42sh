/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_shell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/01/20 12:41:02 by gbourgeo          #+#    #+#             */
/*   Updated: 2014/03/27 18:41:54 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

void        ft_free_shell(t_shell *shell)
{
    int     i;

    i = 1;
    ft_restore_termcaps(shell);
    ft_freestr((char **)&shell->bin_path);
    ft_freetab((char ***)&shell->env);
    ft_command_clear(&shell->command);
    ft_history_remove_all(shell->history);
    ft_highlight_remove_all(&shell->highlighted.texts);
    ft_freestr(&shell->highlighted.yank);
    if (shell->fd > 0)
        close(shell->fd);
    while (i < _NSIG)
    {
        if (shell->sigs[i - 1] != SIG_ERR)
            signal(i, shell->sigs[i - 1]);
        i++;
    }
}
