/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_shell_main.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2013/12/28 04:47:21 by gbourgeo          #+#    #+#             */
/*   Updated: 2018/04/06 17:43:04 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_shell.h"
#include <sys/cdefs.h>

t_shell g_shell;

int main(int __unused argc, const char **argv)
{
    extern char * const * const environ;

    ft_shell_init(argv[0], (const char **) environ, &g_shell);
    argv = ft_shell_args_parse(argv + 1, &g_shell);
    ft_shell_args_exec(argv, &g_shell);
    ft_shell_loop(&g_shell);
    ft_shell_exit(&g_shell);
    return (g_shell.status);
}
