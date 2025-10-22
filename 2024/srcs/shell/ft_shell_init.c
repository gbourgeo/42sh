/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_shell_init.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/02/09 03:00:47 by gbourgeo          #+#    #+#             */
/*   Updated: 2014/03/27 18:39:53 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_defines.h"
#include "ft_shell.h"
#include "ft_shell_environ.h"
#include "ft_shell_history.h"
#include "ft_shell_prompt.h"
#include "ft_shell_terminal.h"
#include "libft.h"
#include <signal.h>
#include <stddef.h>

void ft_shell_init(const char *progname, const char **environ, t_shell *shell)
{
    ft_shell_terminal_init(&shell->terminal);
    ft_shell_prompt_init(&shell->prompt);
    ft_shell_history_init(&shell->history);
    shell->command  = NULL;
    shell->yank     = NULL;
    shell->progname = ft_strrchr(progname, '/');
    shell->progname = (shell->progname != NULL) ? shell->progname + 1 : progname;
    ft_shell_env_init(&shell->environ, environ, shell->progname);
    for (size_t iter = 0; iter < _length_of(shell->sigs); iter++)
    {
        shell->sigs[iter] = SIG_ERR;
    }
    shell->options = 0;
    shell->quit    = 0;
    shell->status  = 0;
}
