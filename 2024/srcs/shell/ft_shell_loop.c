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
#include "ft_shell_command.h"
#include "ft_shell_constants.h"
#include "ft_shell_log.h"
#include "ft_shell_prompt.h"
#include "ft_shell_terminal.h"
#include "ft_shell_termkeys.h"
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void ft_shell_loop_interactive(t_shell *shell);
static void ft_shell_loop_noninteractive(t_shell *shell);

void ft_shell_loop(t_shell *shell)
{
    if (TEST_BIT(shell->options, SHELL_INTERACTIVE_MODE))
    {
        ft_shell_loop_interactive(shell);
    }
    else
    {
        ft_shell_loop_noninteractive(shell);
    }
}

static void ft_shell_loop_interactive(t_shell *shell)
{
    uint8_t key[SHELL_KEY_SIZE] = { 0 };
    long    ret                 = 1;

    /* Main loop */
    while (shell->quit == 0 && ret > 0)
    {
        /* Affiche le prompt */
        ft_shell_prompt_print(&shell->prompt, &shell->terminal);
        /* Debug de la Commande */
        ft_shell_command_debug(key, ret);
        ret = read(shell->fd, key, sizeof(key));
        if (ret < 0)
        {
            ft_shell_log(SH_LOG_LEVEL_FATAL, "read: %s", strerror(errno));
            errno = 0;
            break;
        }
        ft_key_analyser(key, (size_t) ret, shell);
    }
}

static void ft_shell_loop_noninteractive(t_shell *shell)
{
    uint8_t *line     = NULL;
    off_t    filesize = 0;
    off_t    ret      = 0;

    filesize = lseek(shell->fd, 0L, SEEK_END);
    if (filesize == 0)
    {
        return;
    }
    if (filesize < 0)
    {
        ft_shell_log(SH_LOG_LEVEL_FATAL, "lseek: %s", strerror(errno));
        errno = 0;
        return;
    }
    ft_shell_log(SH_LOG_LEVEL_DBG, "Seek file: %d bytes", filesize);
    line = (uint8_t *) malloc((size_t) filesize);
    if (line == NULL)
    {
        ft_shell_log(SH_LOG_LEVEL_FATAL, "malloc: %s", strerror(errno));
        errno = 0;
        return;
    }
    ret = lseek(shell->fd, 0L, SEEK_SET);
    if (ret < 0)
    {
        ft_shell_log(SH_LOG_LEVEL_FATAL, "lseek: %s", strerror(errno));
        errno = 0;
        free(line);
        return;
    }
    if (ret != 0)
    {
        ft_shell_log(SH_LOG_LEVEL_FATAL, "lseek: returning to the begining of file failed.");
        free(line);
        return;
    }
    ret = read(shell->fd, line, (size_t) filesize);
    if (ret < 0)
    {
        ft_shell_log(SH_LOG_LEVEL_FATAL, "read: %s", strerror(errno));
        errno = 0;
        free(line);
        return;
    }
    ft_shell_log(SH_LOG_LEVEL_DBG, "Read %d bytes", ret);
    shell->command = ft_shell_command_new(line, NULL, NULL, SHELL_COMMAND_NEW_STEAL_LINE);
    ft_shell_exec_command(shell);
}
