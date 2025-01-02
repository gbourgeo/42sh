/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_shell_options.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/02/09 03:00:47 by gbourgeo          #+#    #+#             */
/*   Updated: 2014/03/27 18:39:53 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_log.h"
#include "ft_printf.h"
#include "ft_shell.h"
#include "libft.h"

const char **ft_shell_options(const char **argv, t_shell *shell)
{
    while (*argv && **argv == '-')
    {
        if (ft_strcmp(*argv, "--") == 0)
        {
            break;
        }
        if (ft_strcmp(*argv, "-d") == 0 || ft_strcmp(*argv, "--debug") == 0)
        {
            shell->options |= SHELL_DEBUG_MODE;
        }
        else if (ft_strcmp(*argv, "-h") == 0 || ft_strcmp(*argv, "--help") == 0)
        {
            ft_printf("%s version %d.%d.%d\n", shell->progname, SHELL_VERSION_MAJOR, SHELL_VERSION_MINOR, SHELL_VERSION_PATCH);
            ft_printf("Usage: %s [option] ...\n", shell->progname);
            ft_printf("       %s [option] script_file ...\n", shell->progname);
            ft_putendl("Options:");
            ft_putendl("\t--debug, -d");
            ft_putendl("\t--help, -h");
            shell->quit = 1;
        }
        else
        {
            ft_log(SH_LOG_LEVEL_FATAL, "Invalid option: %s", *argv);
            break;
        }
        argv++;
    }
    return (argv);
}
