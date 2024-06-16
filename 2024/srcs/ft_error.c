/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/01/08 17:07:59 by gbourgeo          #+#    #+#             */
/*   Updated: 2014/03/05 15:32:39 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_vdprintf.h"
#include "ft_minishell.h"

void        ft_error_fatal(t_shell *e, const char *err_str, ...)
{
    va_list ap;

    ft_putstr_fd(e->progname, STDERR_FILENO);
    ft_putstr_fd(": ", STDERR_FILENO);
    va_start(ap, err_str);
    ft_vdprintf(STDERR_FILENO, err_str, ap);
    va_end(ap);
    ft_putchar_fd('\n', STDERR_FILENO);
    e->status = 127;
    e->quit = 1;
}

void        ft_error(t_shell *e, const char *err_str, ...)
{
    va_list ap;

    ft_putstr_fd(e->progname, STDERR_FILENO);
    ft_putstr_fd(": ", STDERR_FILENO);
    va_start(ap, err_str);
    ft_vdprintf(STDERR_FILENO, err_str, ap);
    va_end(ap);
    ft_putchar_fd('\n', STDERR_FILENO);
    e->status = 1;
}
