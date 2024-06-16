/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_setenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/01/20 23:26:39 by gbourgeo          #+#    #+#             */
/*   Updated: 2017/02/23 05:37:38 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

static int      search_in_env(const char **args, char * const *env)
{
    int     i;

    i = 0;
    if (env)
    {
        while (env[i] != 0)
        {
            if (ft_strcmp(env[i], args[1]) == '=')
                return (i);
            i++;
        }
    }
    return (-1);
}

static void     modify_env(const char **args, char **env_ptr)
{
    char        *modif;

    modif = ft_strjoin(args[1], "=");
    free(*env_ptr);
    *env_ptr = ft_strjoin(modif, args[2]);
    free(modif);
}

static char     **new_env(const char **args, char **env_old)
{
    char    **env_new;
    char    *join;
    int     i;

    i = ft_tablen(env_old);
    if ((env_new = (char**)malloc(sizeof(char*) * (i + 2))) == NULL)
        return (NULL);
    i = 0;
    if (env_old)
    {
        while (env_old[i])
        {
            env_new[i] = env_old[i];
            i++;
        }
        free(env_old);
    }
    join = ft_strjoin(args[1], "=");
    env_new[i] = ft_strjoin(join, args[2]);
    env_new[i + 1] = NULL;
    free(join);
    return (env_new);
}

int             ft_setenv(const char **args, void *e)
{
    int     ret;

    ret = 0;
    if (args[1] == NULL || ft_strchr(args[1], '='))
        ft_error(e, "%s: bad assignment", args[0]);
    else
    {
        if ((ret = search_in_env(args, ((t_shell *)e)->env)) >= 0)
            modify_env(args, (char **)&((t_shell *)e)->env[ret]);
        else
            ((t_shell *)e)->env = new_env(args, (char **)((t_shell *)e)->env);
        if (((t_shell *)e)->env)
            return (0);
    }
    return (1);
}
