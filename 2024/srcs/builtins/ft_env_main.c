/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/01/20 13:02:31 by gbourgeo          #+#    #+#             */
/*   Updated: 2017/02/23 05:37:15 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_defines.h"
#include "ft_dynamic_table.h"
#include "ft_printf.h"
#include "ft_shell.h"
#include "ft_shell_builtin_env.h"
#include "ft_shell_builtins.h"
#include "ft_shell_environ.h"
#include "ft_shell_log.h"
#include "libft.h"
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>

static void ft_env_usage(const char *builtin_name)
{
    ft_printf("Usage : %s [OPTION]... [-] [NAME=VALUE]... [COMMAND [ARG]...]\n", builtin_name);
    ft_printf("Initialise each NAME to VALUE in the environment and execute COMMAND.\n\n");
    ft_printf("\t-i\tStarts with an empty environment\n");
    ft_printf("\t-0\tEnds each line to stdout with NULL, not with newline\n");
    ft_printf("\t-u\tSuppress environment variable\n");
    ft_printf("\t-C\tChange working directory\n");
    ft_printf("\t-v\tPrint detailled informations on each step of treatment\n");
    ft_printf("\t--help\tPrint helps and exit\n\n");
    ft_printf("A simple - implies -i. If no COMMAND is given, prints the resulting environment.\n");
}

static void ft_env_init(t_builtin_env *env, const char *builtin_name)
{
    env->builtin_name = builtin_name;
    env->change_dir   = NULL;
    env->working_dir  = NULL;
    env->unset_env    = NULL;
    env->set_env      = NULL;
    env->option       = ENV_NO_OPTION;
}

static void ft_env_free(t_builtin_env *env)
{
    ft_dynamic_table_free(&env->cpy);
    if (env->working_dir)
    {
        chdir(env->working_dir);
        free((void *) env->working_dir);
    }
    free((void *) env->unset_env);
    free((void *) env->set_env);
}

int ft_env(const char **args, t_shell *shell)
{
    t_builtin_env env;
    size_t        pos  = 0;
    size_t        iter = 0;
    int           ret  = 0;

    ft_env_init(&env, args[0]);
    if (ft_env_option_parser(shell, &env, args, &pos) == -1)
    {
        ft_env_free(&env);
        return (1);
    }
    if (_test_bit(env.option, ENV_PRINT_HELP))
    {
        ft_env_usage(args[0]);
        ft_env_free(&env);
        return (0);
    }
    if (_test_bit(env.option, ENV_NO_ENV))
    {
        if (_test_bit(env.option, ENV_VERBOSE))
        {
            ft_putendl("cleaning environ");
        }
        ft_dynamic_table_free(&env.cpy);
    }
    else if (env.unset_env != NULL)
    {
        iter = 0;
        while (env.unset_env[iter] != NULL)
        {
            if (_test_bit(env.option, ENV_VERBOSE))
            {
                ft_printf("unset:     %s\n", env.unset_env[iter]);
            }
            t_envelem *elem = ft_shell_env_get_elem(&env.cpy, env.unset_env[iter]);
            if (elem != NULL)
            {
                ft_dynamic_table_remove_elem(&env.cpy, elem);
            }
            iter++;
        }
    }
    if (env.set_env != NULL)
    {
        iter = 0;
        while (env.set_env[iter] != NULL)
        {
            if (_test_bit(env.option, ENV_VERBOSE))
            {
                ft_printf("setenv:    %s\n", env.set_env[iter]);
            }
            ft_shell_env_add_envvar(&env.cpy, env.set_env[iter]);
            iter++;
        }
    }
    if (env.change_dir != NULL)
    {
        env.working_dir = getcwd(NULL, 0);
        if (_test_bit(env.option, ENV_VERBOSE))
        {
            ft_printf("chdir:     '%s'\n", env.change_dir);
        }
        if (chdir(env.change_dir) != 0)
        {
            ft_shell_log(SH_LOG_LEVEL_WARN, "%s: cannot change directory to « %s »", args[0], env.change_dir);
            ft_env_free(&env);
            return (1);
        }
    }
    if (!args[pos])
    {
        const char **environ = (const char **) ft_dynamic_table_get_table(&env.cpy);
        if (environ != NULL)
        {
            iter = 0;
            while (environ[iter] != NULL)
            {
                if (_test_bit(env.option, ENV_PRINT_NO_NEWLINE))
                {
                    ft_putstr(environ[iter]);
                }
                else
                {
                    ft_putendl(environ[iter]);
                }
                iter++;
            }
        }
    }
    else
    {
        if (_test_bit(env.option, ENV_VERBOSE))
        {
            iter = 0;
            ft_printf("executing: %s\n", args[pos + iter]);
            while (args[pos + iter])
            {
                ft_printf("    arg[%d]= « %s »\n", iter, args[pos + iter]);
                iter++;
            }
        }
        env.save              = shell->environ.public;
        shell->environ.public = env.cpy;
        ret                   = check_and_exec(&args[pos], shell);
        shell->environ.public = env.save;
    }
    ft_env_free(&env);
    return (ret);
}
