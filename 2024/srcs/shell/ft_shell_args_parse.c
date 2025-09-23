/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_shell_args_parse.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/02/09 03:00:47 by gbourgeo          #+#    #+#             */
/*   Updated: 2014/03/27 18:39:53 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_defines.h"
#include "ft_printf.h"
#include "ft_shell.h"
#include "ft_shell_constants.h"
#include "ft_shell_log.h"
#include "libft.h"
#include <stddef.h>

static void ft_print_usage(t_shell *shell)
{
    ft_printf("%s shell version %d.%d.%d\n\n"
              "Usage: %1$s [option] ...\n"
              "       %1$s [option] script_file ...\n\n"
              "Options:\n"
              "\t--debug, -d\tPrints debug informations during execution.\n"
              "\t--help, -h\tShows this help and exit.\n",
              shell->progname,
              SHELL_VERSION_MAJOR,
              SHELL_VERSION_MINOR,
              SHELL_VERSION_PATCH);
}

const char **ft_shell_args_parse(const char **argv, t_shell *shell)
{
    while (*argv != NULL && **argv == '-')
    {
        if (ft_strcmp(*argv, "--") == 0)
        {
            return (argv + 1);
        }
        if (ft_strcmp(*argv, "-d") == 0 || ft_strcmp(*argv, "--debug") == 0)
        {
            ASSIGN_BIT(shell->options, SHELL_DEBUG_MODE);
            ft_shell_log(SH_LOG_LEVEL_DBG, "Debug mode activated");
        }
        else if (ft_strcmp(*argv, "-h") == 0 || ft_strcmp(*argv, "--help") == 0)
        {
            ft_print_usage(shell);
            shell->quit = 1;
            return (NULL);
        }
        else
        {
            ft_shell_log(SH_LOG_LEVEL_FATAL, "Invalid option: %s", *argv);
            return (NULL);
        }
        argv++;
    }
    return (argv);
}
