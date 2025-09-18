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

#include "ft_defines.h"
#include "ft_shell.h"
#include "ft_shell_builtins.h"
#include "libft.h"
#include <stddef.h>
#include <unistd.h>

/**
 * @brief Parse les arguments du builtin "echo".
 * Une option commence forcèment par le charactère '-'.
 * Les options prise en charge sont :
 *
 * * -n : Ne termine pas l'exécution par un retour à la ligne.
 *
 * * -e : Interprète les séquences d'échappement suivante :
 *          '\a'    alert (bell),
 *          '\b'    backspace,
 *          '\c'    suppress trailing line,
 *          '\e'    an escape character,
 *          '\f'    form feed,
 *          '\n'    new line,
 *          '\r'    carriage return,
 *          '\t'    horizontal tab,
 *          '\v'    vertical tab,
 *          '\\'    backslash,
 *          '\0nnn' the eight-bit character whose value is the octal value nnn (0 to 3 local digits),
 *          '\xHH'  the eight-bit character whose value is the hexadecimal value HH (1 or 2 hex digits),
 *
 * * -E : Désactive l'interprétation des séquences d'échappement (sur les systèmes le faisant par défaut).
 * @param args      Liste des arguments
 * @param pos[out]  Position du premier argument n'étant pas une option dans la liste
 * @return Les options du builtin.
 */
static echo_e echo_parse_options(const char **args, size_t *pos)
{
    echo_e option_ret = ECHO_END_WITH_NEWLINE;
    size_t iter       = 0;

    while (args[iter] != NULL)
    {
        if (args[iter][0] != '-')
        {
            break;
        }

        echo_e option_save = option_ret;
        size_t jter        = 1;

        while (args[iter][jter])
        {
            if (args[iter][jter] == 'n')
            {
                REMOVE_BIT(option_save, ECHO_END_WITH_NEWLINE);
            }
            else if (args[iter][jter] == 'e')
            {
                ASSIGN_BIT(option_save, ECHO_INTERPRET_ESCAPE_CHAR);
            }
            else if (args[iter][jter] == 'E')
            {
                REMOVE_BIT(option_save, ECHO_INTERPRET_ESCAPE_CHAR);
            }
            else
            {
                *pos = iter;
                return (option_ret);
            }
            jter++;
        }
        TEST_BIT(option_save, ECHO_END_WITH_NEWLINE) ?
            ASSIGN_BIT(option_ret, ECHO_END_WITH_NEWLINE) :
            REMOVE_BIT(option_ret, ECHO_END_WITH_NEWLINE);
        TEST_BIT(option_save, ECHO_INTERPRET_ESCAPE_CHAR) ?
            ASSIGN_BIT(option_ret, ECHO_INTERPRET_ESCAPE_CHAR) :
            REMOVE_BIT(option_ret, ECHO_INTERPRET_ESCAPE_CHAR);
        iter++;
    }
    *pos = iter;
    return (option_ret);
}

int ft_echo(const char **args, t_shell *shell _unused)
{
    size_t iter   = 0;
    echo_e option = echo_parse_options(args + 1, &iter);

    while (args[iter])
    {
        ft_putstr(args[iter]);
        write(STDOUT_FILENO, " ", 1);
        iter++;
    }
    if (TEST_BIT(option, ECHO_END_WITH_NEWLINE))
    {
        write(STDOUT_FILENO, "\n", 1);
    }
    return (0);
}
