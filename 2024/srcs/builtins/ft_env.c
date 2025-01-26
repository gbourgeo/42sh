/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/01/20 13:02:31 by gbourgeo          #+#    #+#             */
/*   Updated: 2017/02/23 05:37:15 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_builtins.h"
#include "ft_dprintf.h"
#include "ft_log.h"
#include "ft_printf.h"
#include "libft.h"
#include <stdlib.h>
#include <unistd.h>

static const char **ft_env_add_var_to_table(char **old_table, const char *var)
{
    const char **new_table = NULL;
    size_t       tablen    = 0;

    tablen    = ft_tablen((const char **)old_table);
    new_table = (const char **) malloc((tablen + 2) * sizeof(*new_table));
    if (new_table == NULL)
    {
        return (NULL);
    }
    new_table[tablen + 1] = NULL;
    new_table[tablen]     = var;
    while (tablen--)
    {
        new_table[tablen] = old_table[tablen];
    }
    free((void *)old_table);
    return (new_table);
}

static char **ft_env_remove_from_env(const char *name, char **env)
{
    size_t iter = 0;

    if (env != NULL)
    {
        while (env[iter] != NULL)
        {
            if (ft_strcmp(env[iter], name) == '=')
            {
                free(env[iter]);
                while (env[iter + 1] != NULL)
                {
                    env[iter] = env[iter + 1];
                    iter++;
                }
                env[iter] = NULL;
                break;
            }
            iter++;
        }
    }
    return (env);
}

static char **ft_env_add_to_env(const char *name, char **env)
{
    size_t iter = 0;

    if (env != NULL)
    {
        while (env[iter] != NULL)
        {
            if (ft_strcmp(env[iter], name) == '=')
            {
                free(env[iter]);
                env[iter] = ft_strdup(name);
                return (env);
            }
            iter++;
        }
    }
    env = (char **) ft_env_add_var_to_table(env, ft_strdup(name));
    return (env);
}

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

static int ft_env_parse_option(t_shell *e, t_env *env, const char **args, int *pos)
{
    size_t i = 0;
    size_t j = 0;

    env->cpy = ft_tabdup((const char **)e->global_env);
    if (env->cpy == NULL)
    {
        return (1);
    }
    i = 1;
    while (args[i] && args[i][0] == '-')
    {
        j = 1;
        if (args[i][j] == '\0')
        {
            env->option |= ENV_NO_ENV;
        }
        while (args[i][j])
        {
            if (args[i][j] == 'i')
            {
                env->option |= ENV_NO_ENV;
            }
            else if (args[i][j] == '0')
            {
                env->option |= ENV_PRINT_NO_NEWLINE;
            }
            else if (args[i][j] == 'u')
            {
                if (args[i][j + 1] == '\0' && args[i + 1] == NULL)
                {
                    ft_log(SH_LOG_LEVEL_WARN, "%s: option needs argument -- %c", args[0], args[i][j]);
                    ft_dprintf(STDERR_FILENO, "use \" %s --help \" for more informations\n", args[0]);
                    return (1);
                }
                if (args[i][j + 1] != '\0')
                {
                    env->unset_env = ft_env_add_var_to_table((char **) env->unset_env, &args[i][j + 1]);
                }
                else
                {
                    env->unset_env = ft_env_add_var_to_table((char **) env->unset_env, args[i + 1]);
                    i++;
                }
                break;
            }
            else if (args[i][j] == 'C')
            {
                if (args[i][j + 1] == '\0' && args[i + 1] == NULL)
                {
                    ft_log(SH_LOG_LEVEL_WARN, "%s: option needs argument -- %c", args[0], args[i][j]);
                    ft_dprintf(STDERR_FILENO, "use \" %s --help \" for more informations\n", args[0]);
                    return (1);
                }
                if (args[i][j + 1] != '\0')
                {
                    env->change_dir = &args[i][j + 1];
                }
                else
                {
                    env->change_dir = args[i + 1];
                    i++;
                }
                break;
            }
            else if (args[i][j] == 'v')
            {
                env->option |= ENV_VERBOSE;
            }
            else if (args[i][j] == '-')
            {
                if (j != 1)
                {
                    ft_log(SH_LOG_LEVEL_WARN, "%s: illegal option -- %c", args[0], args[i][j]);
                    ft_dprintf(STDERR_FILENO, "use \" %s --help \" for more informations\n", args[0]);
                    return (1);
                }
                if (args[i][j + 1] == '\0')
                {
                    break;
                }
                else if (ft_strcmp(args[i], "--help") == 0)
                {
                    env->option |= ENV_PRINT_HELP;
                    return (0);
                }
                else
                {
                    ft_log(SH_LOG_LEVEL_WARN, "%s: unrecognised option -- %s", args[0], args[i]);
                    ft_dprintf(STDERR_FILENO, "use \" %s --help \" for more informations\n", args[0]);
                    return (1);
                }
            }
            else
            {
                ft_log(SH_LOG_LEVEL_WARN, "%s: illegal option -- %c", args[0], args[i][j]);
                ft_dprintf(STDERR_FILENO, "Use \" %s --help \" for more informations\n", args[0]);
                return (1);
            }
            j++;
        }
        i++;
    }
    while (args[i] && ft_strchr(args[i], '=') != NULL)
    {
        env->set_env = ft_env_add_var_to_table((char **) env->set_env, args[i]);
        i++;
    }
    *pos = i;
    return (0);
}

static void ft_env_init(t_env *env)
{
    env->cpy         = NULL;
    env->save        = NULL;
    env->change_dir  = NULL;
    env->working_dir = NULL;
    env->unset_env   = NULL;
    env->set_env     = NULL;
    env->option      = ENV_NO_OPTION;
}

static void ft_env_free(t_env *env)
{
    ft_freetab((char ***) &env->cpy);
    if (env->working_dir)
    {
        chdir(env->working_dir);
        free((char *) env->working_dir);
    }
    free(env->unset_env);
    free(env->set_env);
}

int ft_env(const char **args, t_shell *shell)
{
    t_env env;
    int   pos;
    int   i;

    ft_env_init(&env);
    if (ft_env_parse_option(shell, &env, args, &pos) != 0)
    {
        ft_env_free(&env);
        return (1);
    }
    if (env.option & ENV_PRINT_HELP)
    {
        ft_env_usage(args[0]);
        ft_env_free(&env);
        return (0);
    }
    if (env.option & ENV_NO_ENV)
    {
        if (env.option & ENV_VERBOSE)
        {
            ft_putendl("cleaning environ");
        }
        ft_tabdel((char ***) &env.cpy);
    }
    else if (env.unset_env != NULL)
    {
        i = 0;
        while (env.unset_env[i] != NULL)
        {
            if (env.option & ENV_VERBOSE)
            {
                ft_printf("unset:     %s\n", env.unset_env[i]);
            }
            env.cpy = ft_env_remove_from_env(env.unset_env[i], (char **) env.cpy);
            i++;
        }
    }
    if (env.set_env != NULL)
    {
        i = 0;
        while (env.set_env[i] != NULL)
        {
            if (env.option & ENV_VERBOSE)
            {
                ft_printf("setenv:    %s\n", env.set_env[i]);
            }
            env.cpy = ft_env_add_to_env(env.set_env[i], (char **) env.cpy);
            i++;
        }
    }
    if (env.change_dir != NULL)
    {
        env.working_dir = getcwd(NULL, 0);
        if (env.option & ENV_VERBOSE)
        {
            ft_printf("chdir:     '%s'\n", env.change_dir);
        }
        if (chdir(env.change_dir) != 0)
        {
            ft_log(SH_LOG_LEVEL_WARN, "%s: cannot change directory to « %s »", args[0], env.change_dir);
            ft_env_free(&env);
            return (1);
        }
    }
    if (!args[pos])
    {
        if (env.option & ENV_PRINT_NO_NEWLINE)
        {
            if (env.cpy)
            {
                i = 0;
                while (env.cpy[i])
                {
                    ft_putstr(env.cpy[i]);
                    i++;
                }
            }
        }
        else
        {
            ft_puttab((const char **)env.cpy);
        }
    }
    else
    {
        if (env.option & ENV_VERBOSE)
        {
            i = 0;
            ft_printf("executing: %s\n", args[pos + i]);
            while (args[pos + i])
            {
                ft_printf("    arg[%d]= « %s »\n", i, args[pos + i]);
                i++;
            }
        }
        env.save          = shell->global_env;
        shell->global_env = env.cpy;
        i                 = check_and_exec(&args[pos], shell);
        shell->global_env = env.save;
    }
    ft_env_free(&env);
    return (i);
}
