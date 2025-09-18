/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_shell_loop.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/02/09 03:00:47 by gbourgeo          #+#    #+#             */
/*   Updated: 2014/03/27 18:39:53 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_defines.h"
#include "ft_shell.h"
#include "ft_shell_constants.h"
#include "ft_shell_log.h"
#include "ft_shell_prompt.h"
#include "ft_shell_terminal.h"
#include "ft_shell_termkeys.h"
#include "libft.h"
#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

void ft_shell_loop(t_shell *shell)
{
    uint8_t key[MAX_KEY_SIZE] = { 0 };
    long    ret               = 1;

    /* Main loop */
    while (shell->quit == 0 && ret > 0)
    {
        if (TEST_BIT(shell->options, SHELL_INTERACTIVE_MODE))
        {
            /* Affiche le prompt */
            ft_shell_prompt_print(&shell->prompt, &shell->terminal);
            /* Debug */
            debug_command_line(key, sizeof(key), shell);
        }
        ft_memset(key, 0, sizeof(key));
        ret = read(shell->terminal.fd, key, sizeof(key));
        if (ret < 0)
        {
            ft_log(SH_LOG_LEVEL_FATAL, "read: %s", strerror(errno));
            break;
        }
        ft_key_analyser(key, (size_t) ret, shell);
    }
}
