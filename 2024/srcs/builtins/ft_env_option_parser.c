/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_option_parser.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/01/20 13:02:31 by gbourgeo          #+#    #+#             */
/*   Updated: 2017/02/23 05:37:15 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_defines.h"
#include "ft_shell.h"
#include "ft_shell_builtin_env.h"
#include "ft_shell_log.h"
#include "libft.h"
#include <stddef.h>
#include <stdlib.h>

/**
 * @brief Gestion de l'option 'i' du builtin "env".
 *
 * Active le bit ENV_NO_ENV aux options de l'Environnement du builtin.
 *
 * @param env       Environnement du builtin "env"
 * @param arg       Argument
 * @param pos       Position dans l'argument
 * @param args_list Liste des arguments
 * @return 0.
 */
static int ft_env_i_handler(
    t_builtin_env         *env,
    const char *arg        _unused,
    size_t pos             _unused,
    const char **args_list _unused)
{
    ASSIGN_BIT(env->option, ENV_NO_ENV);
    return (0);
}

/**
 * @brief Gestion de l'option '0' du builtin "env".
 *
 * Active le bit ENV_PRINT_NO_NEWLINE aux options de l'Environnement du builtin.
 *
 * @param env       Environnement du builtin "env"
 * @param arg       Argument
 * @param pos       Position dans l'argument
 * @param args_list Liste des arguments
 * @return 0.
 */
static int ft_env_zero_handler(
    t_builtin_env         *env,
    const char *arg        _unused,
    size_t pos             _unused,
    const char **args_list _unused)
{
    ASSIGN_BIT(env->option, ENV_PRINT_NO_NEWLINE);
    return (0);
}

/**
 * @brief Gestion de l'option 'u' du builtin "env".
 *
 * Ajoute l'argument de l'option à la liste des variables d'Environnement à supprimer.
 *
 * @param env       Environnement du builtin "env"
 * @param arg       Argument
 * @param pos       Position dans l'argument
 * @param args_list Liste des arguments
 * @return -1 si erreur,
 *          1 si la liste d'argument doit ê tre incrémentée en sortie de fonction,
 *          0 autrement.
 */
static int ft_env_u_handler(
    t_builtin_env *env,
    const char    *arg,
    size_t pos     _unused,
    const char   **args_list)
{
    if (arg[1] != '\0') /* L'argument est collé à l'option */
    {
        env->unset_env = ft_env_add_var_to_table((char **) env->unset_env, arg + 1);
        return (0);
    }
    if (args_list[1] != NULL) /* L'argument est le prochain argument */
    {
        env->unset_env = ft_env_add_var_to_table((char **) env->unset_env, args_list[1]);
        return (1);
    }
    /* Pas d'argument */
    ft_shell_log(SH_LOG_LEVEL_WARN,
           "%s: option needs argument -- %c\nuse \" %1$s --help \" for more informations",
           env->builtin_name,
           *arg);
    return (-1);
}

/**
 * @brief Gestion de l'option 'C' du builtin "env".
 *
 * Récupère l'argument de l'option considéré comme nouveau répertoire d'exécution.
 *
 * @param env       Environnement du builtin "env"
 * @param arg       Argument
 * @param pos       Position dans l'argument
 * @param args_list Liste des arguments
 * @return -1 si erreur,
 *          1 si la liste d'argument doit ê tre incrémentée en sortie de fonction,
 *          0 autrement.
 */
static int ft_env_c_handler(
    t_builtin_env *env,
    const char    *arg,
    size_t pos     _unused,
    const char   **args_list)
{
    if (arg[1] != '\0') /* L'argument est collé à l'option */
    {
        env->change_dir = arg + 1;
        return (0);
    }
    if (args_list[1] != NULL) /* L'argument est le prochain argument */
    {
        env->change_dir = args_list[1];
        return (1);
    }
    /* Pas d'argument */
    ft_shell_log(SH_LOG_LEVEL_WARN,
           "%s: option needs argument -- %c\nuse \" %1$s --help \" for more informations",
           env->builtin_name,
           *arg);
    return (-1);
}

/**
 * @brief Gestion de l'option '0' du builtin "env".
 *
 * Active le bit ENV_VERBOSE aux options de l'Environnement du builtin.
 *
 * @param env       Environnement du builtin "env"
 * @param arg       Argument
 * @param pos       Position dans l'argument
 * @param args_list Liste des arguments
 * @return 0.
 */
static int ft_env_v_handler(
    t_builtin_env         *env,
    const char *arg        _unused,
    size_t pos             _unused,
    const char **args_list _unused)
{
    ASSIGN_BIT(env->option, ENV_VERBOSE);
    return (0);
}

/**
 * @brief Gestion de l'option '-' du builtin "env".
 *
 * Doit forcément être le premier caractère de l'argument.
 *
 * Sans argument, active le bit ENV_NO_ENV aux options de l'Environnement du builtin.
 *
 * S'il est suivi des caractères "-help", active le bit ENV_PRINT_HELP aux options de l'Environnement du builtin.
 *
 * @param env       Environnement du builtin "env"
 * @param arg       Argument
 * @param pos       Position dans l'argument
 * @param args_list Liste des arguments
 * @return -1 si erreur, 0 autrement.
 */
static int ft_env_dash_handler(
    t_builtin_env         *env,
    const char            *arg,
    size_t                 pos,
    const char **args_list _unused)
{
    if (pos != 1) /* Pas le premier caractère */
    {
        ft_shell_log(SH_LOG_LEVEL_WARN,
               "%s: illegal option -- %c\nuse \" %1$s --help \" for more informations",
               env->builtin_name,
               *arg);
        return (-1);
    }
    if (arg[1] == '\0') /* Pas d'autres caractères, agit comme l'option -i */
    {
        ASSIGN_BIT(env->option, ENV_NO_ENV);
        return (0);
    }
    if (ft_strcmp(arg, "-help") == 0)
    {
        ASSIGN_BIT(env->option, ENV_PRINT_HELP);
        return (0);
    }
    ft_shell_log(SH_LOG_LEVEL_WARN,
           "%s: unrecognised option -- -%s\nuse \" %1$s --help \" for more informations",
           env->builtin_name,
           arg);
    return (-1);
}

int ft_env_option_parser(t_shell *shell, t_builtin_env *env, const char **args, size_t *pos)
{
    static t_builtin_env_opt options[] = {
        { .value = 'i', .handler = ft_env_i_handler    },
        { .value = '0', .handler = ft_env_zero_handler },
        { .value = 'u', .handler = ft_env_u_handler    },
        { .value = 'C', .handler = ft_env_c_handler    },
        { .value = 'v', .handler = ft_env_v_handler    },
        { .value = '-', .handler = ft_env_dash_handler },
    };
    const char *arg   = args[1];
    size_t      iopt  = 0;
    size_t      iargs = 1;
    size_t      iarg  = 0;
    int         ret   = 0;

    env->cpy = ft_tabdup((const char **) shell->environ.public);
    if (env->cpy == NULL)
    {
        ft_shell_log(SH_LOG_LEVEL_ERR,
               "%s: global environment duplication failed.",
               env->builtin_name);
        return (-1);
    }
    while (arg != NULL && arg[0] == '-') /* Parcours de la liste des arguments */
    {
        iarg = 1;
        while (arg[iarg] != '\0') /* Parcours des caractères de l'argument */
        {
            iopt = 0;
            while (iopt < sizeof(options) / sizeof(options[0])) /* Parcours des options du builtin */
            {
                if (arg[iarg] == options[iopt].value)
                {
                    ret = options[iopt].handler(env, arg + iarg, iarg, args + iargs);
                    if (ret == 1) /* Incrèment dans la liste des arguments */
                    {
                        iargs++;
                    }
                    else if (ret == -1) /* Erreur */
                    {
                        return (-1);
                    }
                    break;
                }
                iopt++;
            }
            if (iopt == sizeof(options) / sizeof(options[0])) /* Mauvaise option */
            {
                ft_shell_log(SH_LOG_LEVEL_WARN,
                       "%s: illegal option -- %c\nuse \" %1$s --help \" for more informations",
                       env->builtin_name,
                       arg[iarg]);
                return (-1);
            }
            iarg++;
        }
        iargs++;
        arg = args[iargs];
    }
    while (args[iargs] != NULL && ft_strchr(args[iargs], '=') != NULL)
    {
        env->set_env = ft_env_add_var_to_table((char **) env->set_env, args[iargs]);
        iargs++;
    }
    *pos = iargs;
    return (0);

    // while (args[iter] && args[iter][0] == '-')
    // {
    //     jter = 1;
    //     if (args[iter][jter] == '\0')
    //     {
    //         ASSIGN_BIT(env->option, ENV_NO_ENV);
    //     }
    //     while (args[iter][jter] != '\0')
    //     {
    //         if (args[iter][jter] == 'i')
    //         {
    //             ASSIGN_BIT(env->option, ENV_NO_ENV);
    //         }
    //         else if (args[iter][jter] == '0')
    //         {
    //             ASSIGN_BIT(env->option, ENV_PRINT_NO_NEWLINE);
    //         }
    //         else if (args[iter][jter] == 'u')
    //         {
    //             if (args[iter][jter + 1] == '\0' && args[iter + 1] == NULL)
    //             {
    //                 ft_shell_log(SH_LOG_LEVEL_WARN, "%s: option needs argument -- %c", args[0], args[iter][jter]);
    //                 ft_dprintf(STDERR_FILENO, "use \" %s --help \" for more informations\n", args[0]);
    //                 return (1);
    //             }
    //             if (args[iter][jter + 1] != '\0')
    //             {
    //                 env->unset_env = ft_env_add_var_to_table((char **) env->unset_env, &args[iter][jter + 1]);
    //             }
    //             else
    //             {
    //                 env->unset_env = ft_env_add_var_to_table((char **) env->unset_env, args[iter + 1]);
    //                 iter++;
    //             }
    //             break;
    //         }
    //         else if (args[iter][jter] == 'C')
    //         {
    //             if (args[iter][jter + 1] == '\0' && args[iter + 1] == NULL)
    //             {
    //                 ft_shell_log(SH_LOG_LEVEL_WARN, "%s: option needs argument -- %c", args[0], args[iter][jter]);
    //                 ft_dprintf(STDERR_FILENO, "use \" %s --help \" for more informations\n", args[0]);
    //                 return (1);
    //             }
    //             if (args[iter][jter + 1] != '\0')
    //             {
    //                 env->change_dir = &args[iter][jter + 1];
    //             }
    //             else
    //             {
    //                 env->change_dir = args[iter + 1];
    //                 iter++;
    //             }
    //             break;
    //         }
    //         else if (args[iter][jter] == 'v')
    //         {
    //             ASSIGN_BIT(env->option, ENV_VERBOSE);
    //         }
    //         else if (args[iter][jter] == '-')
    //         {
    //             if (jter != 1)
    //             {
    //                 ft_shell_log(SH_LOG_LEVEL_WARN, "%s: illegal option -- %c", args[0], args[iter][jter]);
    //                 ft_dprintf(STDERR_FILENO, "use \" %s --help \" for more informations\n", args[0]);
    //                 return (1);
    //             }
    //             if (args[iter][jter + 1] == '\0')
    //             {
    //                 break;
    //             }
    //             if (ft_strcmp(args[iter], "--help") == 0)
    //             {
    //                 ASSIGN_BIT(env->option, ENV_PRINT_HELP);
    //                 return (0);
    //             }
    //             ft_shell_log(SH_LOG_LEVEL_WARN, "%s: unrecognised option -- %s", args[0], args[iter]);
    //             ft_dprintf(STDERR_FILENO, "use \" %s --help \" for more informations\n", args[0]);
    //             return (1);
    //         }
    //         else
    //         {
    //             ft_shell_log(SH_LOG_LEVEL_WARN, "%s: illegal option -- %c", args[0], args[iter][jter]);
    //             ft_dprintf(STDERR_FILENO, "Use \" %s --help \" for more informations\n", args[0]);
    //             return (1);
    //         }
    //         jter++;
    //     }
    //     iter++;
    // }
    // while (args[iter] && ft_strchr(args[iter], '=') != NULL)
    // {
    //     env->set_env = ft_env_add_var_to_table((char **) env->set_env, args[iter]);
    //     iter++;
    // }
    // *pos = iter;
    // return (0);
}

const char **ft_env_add_var_to_table(char **table, const char *var)
{
    const char **new_table = NULL;
    size_t       tablen    = 0;

    tablen    = ft_tablen((const char **) table);
    new_table = (const char **) malloc((tablen + 2) * sizeof(*new_table));
    if (new_table == NULL)
    {
        return (NULL);
    }
    new_table[tablen + 1] = NULL;
    new_table[tablen]     = var;
    while (tablen-- != 0)
    {
        new_table[tablen] = table[tablen];
    }
    free((void *) table);
    return (new_table);
}
