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

#include "ft_dprintf.h"
#include "ft_minishell.h"

static void     ft_real_exit(t_shell *shell)
{
    write(STDOUT_FILENO, "exit\n", 5);
    ft_tputs("cd", shell); /* Efface la ligne et celles sous le curseur */
    shell->quit = 1;
    shell->prompt.print = 1;
}

void            ft_exit(const char **args, void *e)
{
    int     i;

    i = 0;
    if (!args || !args[0] || !args[1])
        return (ft_real_exit(e));
    if (!args[2])
    {
        while (args[1][i] != '\0')
        {
            if (!ft_isdigit(args[1][i]))
            {
                ((t_shell *)e)->status = 255;
                ft_error(e, "%s: Non numeric value detected.", args[0]);
                return (ft_real_exit(e));
            }
            i++;
        }
        ((t_shell *)e)->status = ft_atoi(args[1]);
        return (ft_real_exit(e));
    }
    ft_error(e, "%s: too many arguments.", args[0]);
    ((t_shell *)e)->status = 1;
}
