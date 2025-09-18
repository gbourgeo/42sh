/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/01/20 12:12:34 by gbourgeo          #+#    #+#             */
/*   Updated: 2014/03/27 19:21:14 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_shell_builtins.h"
#include "ft_shell.h"
#include "libft.h"
#include <stddef.h>

const char *ft_real_getenv(const char *str, const char **env)
{
    long pos = 0;

    if (str == NULL || env == NULL)
    {
        return (NULL);
    }
    while (env[pos] != NULL)
    {
        if (ft_strcmp(env[pos], str) == '=')
        {
            return (env[pos] + ft_strlen(str) + 1);
        }
        pos++;
    }
    return (NULL);
}

const char *ft_getenv(const char *str, t_shell *shell)
{
    const char *ptr = NULL;

    ptr = ft_real_getenv(str, (const char **) shell->global_env);
    if (ptr == NULL)
    {
        ptr = ft_real_getenv(str, (const char **) shell->internal_env);
    }
    return (ptr);
}
