/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2013/12/28 04:47:21 by gbourgeo          #+#    #+#             */
/*   Updated: 2018/04/06 17:43:04 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_shell.h"

t_shell g_shell; /* Structure globale de notre shell */

int     main(int ac, const char **av)
{
    extern char * const *environ;

    (void) ac;
    ft_shell_init(av[0], environ, &g_shell);
    av = ft_shell_options(av + 1, &g_shell);
    if (g_shell.quit == 0)
        ft_shell_exec(av, &g_shell);
    ft_shell_exit(&g_shell);
    return (g_shell.status);
}
