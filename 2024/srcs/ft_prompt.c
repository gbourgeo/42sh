/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_prompt.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/03/26 05:42:45 by gbourgeo          #+#    #+#             */
/*   Updated: 2014/03/26 05:43:15 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "ft_minishell.h"

/**
 * Récupère la position du curseur.
 * Ecrit "\e[6n" dans le terminal et récupère le code de retour sous la forme :
 * "\e[XX;YY" où XX est la ligne et YY la colonne.
 * @param[in] shell Structure interne du shell
*/
void        ft_get_cursor_position(t_shell *shell)
{
    char    buff[16] = { 0 };

    write(STDOUT_FILENO, "\e[6n", 4);
    read(STDOUT_FILENO, buff, sizeof(buff));
    shell->terminal.current_line = ft_atoi(buff + 2) - 1;
    shell->terminal.current_column = ft_atoi(ft_strchr(buff + 2, ';') + 1) - 1;
    shell->terminal.start_line = shell->terminal.current_line;
    shell->terminal.start_column = shell->terminal.current_column;
}

/**
 * Affiche le prompt du shell interactif.
 * @param[in] shell Structure interne du shell
*/
void        ft_prompt(t_shell *shell)
{
    const char  *pwd;
    const char  *home;
    const char  *tmp;

    shell->prompt.print = 0;
    pwd = ft_getenv("PWD", shell->env);
    home = ft_getenv("HOME", shell->env);
    tmp = "";
    if (ft_strncmp(pwd, home, ft_strlen(home)) == 0)
    {
        tmp = "~";
        pwd += ft_strlen(home);
    }
    ft_tputs("cd", shell); /* Efface le caractère sous et après le curseur */
    shell->prompt.len = ft_printf("\e[37m%s \e[33m%s\e[0m \e[31m%s%s \e[0m> ",
        shell->progname,
        ft_getenv("USER", shell->env),
        tmp,
        pwd) - 23;
    ft_get_cursor_position(shell);
}
