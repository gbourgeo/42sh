/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init_shell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/02/09 03:00:47 by gbourgeo          #+#    #+#             */
/*   Updated: 2014/03/27 18:39:53 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>

#include "ft_minishell.h"

void        ft_get_terminal_size(t_shell *e)
{
    struct winsize  w;

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    e->terminal.max_column = w.ws_col - 1;
    e->terminal.max_line = w.ws_row - 1;
}
