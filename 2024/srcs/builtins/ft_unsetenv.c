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

#include "ft_shell.h"
#include "ft_shell_builtins.h"
#include "ft_shell_log.h"
#include "libft.h"
#include <stddef.h>
#include <stdlib.h>

static int new_env(const char **args, char **env)
{
    size_t iter = 0;

    if (env == NULL)
    {
        return (0);
    }
    while (env[iter] != NULL)
    {
        if (ft_strcmp(env[iter], args[1]) == '=')
        {
            free(env[iter]);
            while (env[iter + 1] != NULL)
            {
                env[iter] = env[iter + 1];
                iter++;
            }
            env[iter] = NULL;
        }
        iter++;
    }
    return (0);
}

int ft_unsetenv(const char **args, t_shell *shell)
{
    if (!args[1])
    {
        ft_log(SH_LOG_LEVEL_WARN, "%s: No assignment", args[0]);
    }
    else
    {
        return (new_env(args, shell->global_env));
    }
    return (1);
}
