/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unsetenv.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/01/20 23:26:00 by gbourgeo          #+#    #+#             */
/*   Updated: 2014/03/27 19:21:38 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

static int      new_env(const char **args, char **env)
{
    int     i;

    i = 0;
    if (env == NULL)
        return (0);
    while (env[i] != NULL)
    {
        if (ft_strcmp(env[i], args[1]) == '=')
        {
            free(env[i]);
            while (env[i + 1] != NULL)
            {
                env[i] = env[i + 1];
                i++;
            }
            env[i] = NULL;
        }
        i++;
    }
    return (0);
}

int             ft_unsetenv(const char **args, void *e)
{
    if (!args[1])
        ft_error(e, "%s: No assignment", args[0]);
    else
        return (new_env(args, (char **)((t_shell *)e)->env));
    return (1);
}
