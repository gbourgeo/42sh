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

#include "ft_minishell.h"

const char      *ft_getenv(const char *str, char * const *env)
{
    int     i;

    i = 0;
    if (env && str)
    {
        while (env[i] != NULL)
        {
            if (ft_strcmp(env[i], str) == '=')
                return (env[i] + ft_strlen(str) + 1);
            ++i;
        }
    }
    return (NULL);
}
