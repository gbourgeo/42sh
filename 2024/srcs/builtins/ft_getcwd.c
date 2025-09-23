/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getcwd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/02/15 06:07:29 by gbourgeo          #+#    #+#             */
/*   Updated: 2014/02/15 22:38:08 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_defines.h"
#include "ft_shell.h"
#include "ft_shell_builtins.h"
#include "ft_shell_log.h"
#include <errno.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>

char *ft_getcwd(const char *dir _unused, t_shell *shell)
{
    char       *pwd = NULL;
    const char *tmp = ft_getenv("PWD", shell);

    if (tmp == NULL)
    {
        return (NULL);
    }
    pwd = getcwd(NULL, 0);
    if (pwd == NULL)
    {
        ft_shell_log(SH_LOG_LEVEL_WARN, "getcwd: %s", strerror(errno));
    }
    return (pwd);
}
