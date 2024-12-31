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

#include "ft_builtins.h"
#include "ft_log.h"
#include "libft.h"

#include <unistd.h>

char *ft_getcwd(const char *dir, t_shell *shell)
{
    char       *pwd;
    const char *tmp;
    int         full_size;

    pwd       = NULL;
    tmp       = ft_getenv("PWD", shell);
    full_size = ft_strlen(tmp) + ft_strlen(dir);
    if (tmp == NULL)
        return (NULL);
    if ((pwd = getcwd(pwd, full_size)) == NULL)
        ft_log(SH_LOG_LEVEL_WARN, "getcwd: error");
    return (pwd);
}
