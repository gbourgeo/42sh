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

#include "ft_shell.h"
#include "ft_shell_command.h"
#include "ft_shell_history.h"
#include "ft_shell_prompt.h"
#include "ft_shell_terminal.h"
#include "libft.h"
#include <signal.h>
#include <stdlib.h>

void ft_shell_exit(t_shell *shell)
{
    int iter = 1;

    ft_shell_terminal_clear(&shell->terminal, shell->options);
    ft_shell_prompt_clear(&shell->prompt);
    ft_shell_history_save_to_file(&shell->history, shell->command);
    ft_shell_command_delete_list(shell->command);
    free(shell->yank);
    free((void *) shell->bin_path);
    ft_freetab(&shell->global_env);
    ft_freetab(&shell->internal_env);
    while (iter < NSIG)
    {
        if (shell->sigs[iter - 1] != SIG_ERR)
        {
            (void) signal(iter, shell->sigs[iter - 1]);
        }
        iter++;
    }
}
