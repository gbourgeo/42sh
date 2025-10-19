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
#include "ft_shell_environ.h"
#include "ft_shell_log.h"
#include <stddef.h>
#include <stdlib.h>

int ft_unsetenv(const char **args, t_shell *shell)
{
    if (!args[1])
    {
        ft_shell_log(SH_LOG_LEVEL_WARN, "%s: No assignment", args[0]);
        return (1);
    }
    ft_shell_env_remove(&shell->environ, args[1]);
    return (0);
}
