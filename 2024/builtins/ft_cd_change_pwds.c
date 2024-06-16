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

#include "ft_minishell.h"

void        cd_change_pwds(const char *pwd, void *e)
{
    const char  *tmp[4];

    tmp[0] = "setenv";
    tmp[3] = NULL;
    if (ft_getenv("OLDPWD", ((t_shell *)e)->env))
    {
        tmp[1] = "OLDPWD";
        tmp[2] = ft_getenv("PWD", ((t_shell *)e)->env);
        ft_setenv(tmp, e);
    }
    if (ft_getenv("PWD", ((t_shell *)e)->env))
    {
        tmp[1] = "PWD";
        tmp[2] = pwd;
        ft_setenv(tmp, e);
    }
}
