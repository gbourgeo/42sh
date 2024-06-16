/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/02/15 03:43:58 by gbourgeo          #+#    #+#             */
/*   Updated: 2014/05/18 01:01:14 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_echo.h"
#include "ft_minishell.h"

static echo_e   echo_option(const char **args, int *pos)
{
    echo_e  option_ret;
    echo_e  option_save;
    int     i;
    int     j;

    option_ret = ECHO_END_WITH_NEWLINE;
    i = 1;
    while (args[i])
    {
        if (args[i][0] != '-')
            break;
        option_save = option_ret;
        j = 1;
        while (args[i][j])
        {
            if (args[i][j] == 'n')
                option_save &= ~ECHO_END_WITH_NEWLINE;
            else if (args[i][j] == 'e')
                option_save |= (ECHO_INTERPRET_ESCAPE_CHAR);
            else if (args[i][j] == 'E')
                option_save &= ~ECHO_INTERPRET_ESCAPE_CHAR;
            else
            {
                option_save |= (ECHO_BREAK);
                break;
            }
            j++;
        }
        if (option_save & ECHO_BREAK)
            break;
        if (option_save & ECHO_END_WITH_NEWLINE)
            option_ret |= ECHO_END_WITH_NEWLINE;
        else
            option_ret &= ~ECHO_END_WITH_NEWLINE;
        if (option_save & ECHO_INTERPRET_ESCAPE_CHAR)
            option_ret |= ECHO_INTERPRET_ESCAPE_CHAR;
        else
            option_ret &= ~ECHO_INTERPRET_ESCAPE_CHAR;
        i++;
    }
    *pos = i;
    return (option_ret);
}

int             ft_echo(const char **args, void *e)
{
    echo_e  option;
    int     i;

    (void)e;
    option = echo_option(args, &i);
    while (args[i])
    {
        ft_putstr(args[i]);
        write(STDOUT_FILENO, " ", 1);
        i++;
    }
    if (option & ECHO_END_WITH_NEWLINE)
        write(STDOUT_FILENO, "\n", 1);
    return (0);
}
