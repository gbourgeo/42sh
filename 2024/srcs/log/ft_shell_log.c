/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_shell_log.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/01/08 17:07:59 by gbourgeo          #+#    #+#             */
/*   Updated: 2014/03/05 15:32:39 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_shell_log.h"
#include "ft_defines.h"
#include "ft_dprintf.h"
#include "ft_shell.h"
#include "ft_shell_constants.h"
#include "ft_vdprintf.h"
#include <stdarg.h>
#include <stdint.h>
#include <unistd.h>

typedef struct _align(32) s_log
{
    const char *name;
    const char *color;
    int         fd;
    retcode_e   status;
    int         quit;
} t_log;

void ft_shell_log(log_level_e log_level, const char *file, uint32_t line,
                  const char *err_str, ...)
{
    extern t_shell g_shell;
    static t_log   log_print[] = {
        { .fd = STDOUT_FILENO, .name = "DEBUG",   .color = "\033[35m", .status = SHELL_STATUS_OK,      .quit = 0 }, // magenta
        { .fd = STDOUT_FILENO, .name = "INFO",    .color = "\033[37m", .status = SHELL_STATUS_OK,      .quit = 0 }, // white
        { .fd = STDERR_FILENO, .name = "WARNING", .color = "\033[33m", .status = SHELL_STATUS_WARNING, .quit = 0 }, // yellow
        { .fd = STDERR_FILENO, .name = "ERROR",   .color = "\033[31m", .status = SHELL_STATUS_FATAL,   .quit = 0 }, // red
        { .fd = STDERR_FILENO, .name = "FATAL",   .color = "\033[36m", .status = SHELL_STATUS_FATAL,   .quit = 1 }, // cyan
    };
    va_list argp = { 0 };

    if (log_level == SHELL_LOG_LEVEL_DBG && !TEST_BIT(g_shell.options, SHELL_DEBUG_MODE))
    {
        return;
    }
    va_start(argp, err_str);
    if (TEST_BIT(g_shell.options, SHELL_DEBUG_MODE))
    {
        ft_dprintf(log_print[log_level].fd,
                   "%s%s %s: %s:%d ",
                   log_print[log_level].color,
                   log_print[log_level].name,
                   g_shell.progname,
                   file,
                   line);
    }
    else
    {
        ft_dprintf(log_print[log_level].fd,
                   "%s%s %s: ",
                   log_print[log_level].color,
                   log_print[log_level].name,
                   g_shell.progname);
    }
    ft_vdprintf(log_print[log_level].fd, err_str, argp);
    write(log_print[log_level].fd, "\033[0m\n", sizeof("\033[0m\n"));
    va_end(argp);

    g_shell.status = (int) log_print[log_level].status;
    g_shell.quit   = log_print[log_level].quit;
}
