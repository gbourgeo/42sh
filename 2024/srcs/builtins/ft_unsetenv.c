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

#include "ft_builtins.h"
#include "ft_log.h"
#include "libft.h"

static int new_env(const char **args, char **env)
{
    int i;

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

int ft_unsetenv(const char **args, t_shell *shell)
{
    if (!args[1])
        ft_log(SH_LOG_LEVEL_WARN, "%s: No assignment", args[0]);
    else
        return (new_env(args, (char **) shell->global_env));
    return (1);
}
