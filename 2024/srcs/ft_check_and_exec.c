/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check_and_exec.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/02/09 03:19:24 by gbourgeo          #+#    #+#             */
/*   Updated: 2014/05/18 01:19:08 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_builtins.h"
#include "ft_shell.h"
#include "libft.h"

int check_and_exec(const char **args, t_shell *shell)
{
    if (args && *args)
    {
        if (ft_strcmp(*args, "exit") == 0)
            ft_exit(args, shell);
        else if (ft_strcmp(*args, "env") == 0)
            return (ft_env(args, shell));
        else if (ft_strcmp(*args, "setenv") == 0)
            return (ft_setenv(args, shell));
        else if (ft_strcmp(*args, "unsetenv") == 0)
            return (ft_unsetenv(args, shell));
        else if (ft_strcmp(*args, "cd") == 0)
            return (ft_cd(args, shell));
        else if (ft_strcmp(*args, "echo") == 0)
            return (ft_echo(args, shell));
        else
            return (fork_function(args, shell));
    }
    return (shell->status);
}
