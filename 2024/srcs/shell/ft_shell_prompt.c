/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_shell_prompt.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/02/09 03:00:47 by gbourgeo          #+#    #+#             */
/*   Updated: 2014/03/27 18:39:53 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_builtins.h"
#include "ft_printf.h"
#include "ft_prompt.h"
#include "ft_shell.h"
#include "ft_termios.h"
#include "libft.h"

void ft_init_shell_prompt(t_prompt *prompt)
{
    prompt->str   = NULL;
    prompt->len   = 0;
    prompt->print = 0;
}

void ft_shell_prompt(t_shell *shell)
{
    const char *pwd  = NULL;
    const char *home = NULL;
    const char *user = NULL;
    const char *tmp  = NULL;

    pwd              = ft_getenv("PWD", shell);
    home             = ft_getenv("HOME", shell);
    user             = ft_getenv("USER", shell);
    tmp              = "";
    if (ft_strncmp(pwd, home, ft_strlen(home)) == 0)
    {
        tmp = "~";
        pwd += ft_strlen(home);
    }
    ft_term_clear_line_and_under(&shell->terminal); /* Efface le caractère sous et après le curseur */
    shell->prompt.len = ft_printf("\033[37m%s \033[33m%s\033[0m \033[31m%s%s \033[0m> ",
                                  shell->progname,
                                  (user != NULL) ? user : "",
                                  tmp,
                                  pwd)
                      - 23;
    ft_get_cursor_position(&shell->terminal);
    shell->prompt.print = 0;
}
