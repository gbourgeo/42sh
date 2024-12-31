/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_change_pwds.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/02/15 20:44:42 by gbourgeo          #+#    #+#             */
/*   Updated: 2017/02/23 05:36:51 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_builtins.h"

void cd_change_pwds(const char *pwd, t_shell *shell)
{
    const char *tmp[4];

    tmp[0] = "setenv";
    tmp[3] = NULL;
    if (ft_getenv("OLDPWD", shell))
    {
        tmp[1] = "OLDPWD";
        tmp[2] = ft_getenv("PWD", shell);
        ft_setenv(tmp, shell);
    }
    if (ft_getenv("PWD", shell))
    {
        tmp[1] = "PWD";
        tmp[2] = pwd;
        ft_setenv(tmp, shell);
    }
}
