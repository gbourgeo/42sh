/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/01/20 12:41:02 by gbourgeo          #+#    #+#             */
/*   Updated: 2014/03/27 18:41:54 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_builtins.h"
#include "ft_log.h"
#include "libft.h"

#include <unistd.h>

static void ft_real_exit(t_shell *shell)
{
    write(STDOUT_FILENO, "exit\n", 5);
    ft_term_clear_line_and_under(&shell->terminal);
    shell->quit = 1;
}

void ft_exit(const char **args, t_shell *shell)
{
    int i;

    i = 0;
    if (!args || !args[0] || !args[1])
        return (ft_real_exit(shell));
    if (!args[2])
    {
        while (args[1][i] != '\0')
        {
            if (!ft_isdigit(args[1][i]))
            {
                ft_log(SH_LOG_LEVEL_ERR, "%s: Non numeric value detected.", args[0]);
                shell->status = 255;
                return (ft_real_exit(shell));
            }
            i++;
        }
        shell->status = ft_atoi(args[1]);
        return (ft_real_exit(shell));
    }
    ft_log(SH_LOG_LEVEL_WARN, "%s: too many arguments.", args[0]);
    shell->status = 1;
}
