/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_syntax_analysis_.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/02/06 11:22:07 by gbourgeo          #+#    #+#             */
/*   Updated: 2014/03/27 19:59:14 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_shell.h"
#include "ft_shell_log.h"
#include "ft_shell_redirections.h"
#include "libft.h"

static int ft_pipes_analysis(char **args)
{
    int i;

    i = 0;
    if (args == NULL)
    {
        return (1);
    }
    while (args[i] != NULL)
    {
        if (ft_strchr(PIPES, args[i][0]) != NULL)
        {
            if (ft_strlen(args[i]) > 4
                || (args[i][0] == '<' && ft_check_redirect_left(args, i))
                || (args[i][0] == '>' && ft_check_redirect_right(args, i))
                || (args[i][0] == '|' && ft_check_redirect_pipe(args, i)))
            {
                ft_shell_log(SH_LOG_LEVEL_WARN, "parse error near `%s'", args[i]);
                return (1);
            }
        }
        i++;
    }
    return (0);
}

int ft_syntax_analysis(char **args)
{
    int i;

    i = 0;
    if (args == NULL)
    {
        return (1);
    }
    while (args[i] != NULL)
    {
        if (ft_strchr(SEPARATORS, args[i][0]) != NULL)
        {
            if (ft_strlen(args[i]) > 2
                || (ft_strstr(args[i], "||") && ft_check_double_pipe(args, i))
                || (ft_strchr(args[i], ';') && ft_check_semi_colon(args, i))
                || (ft_strstr(args[i], "&&") && ft_check_double_and(args, i)))
            {
                ft_shell_log(SH_LOG_LEVEL_WARN, "parse error near `%s'", args[i]);
                return (1);
            }
        }
        i++;
    }
    return (ft_pipes_analysis(args));
}
