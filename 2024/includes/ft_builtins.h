/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtins.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/01/20 12:12:34 by gbourgeo          #+#    #+#             */
/*   Updated: 2014/03/27 19:21:14 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_BUILTINS_H
#define FT_BUILTINS_H

#include "ft_constants.h"
#include "ft_shell.h"

typedef enum cd_options
{
    CD_RESOLVE_UNDEFINED = 0x0000,
    CD_RESOLVE_BEFORE    = 0x0001,
    CD_RESOLVE_AFTER     = 0X0002,
    CD_BREAK             = 0x1000,
} cd_e;

typedef enum echo_options
{
    ECHO_END_WITH_NEWLINE      = 0x0001,
    ECHO_INTERPRET_ESCAPE_CHAR = 0x0002,
    ECHO_BREAK                 = 0x1000,
} echo_e;

typedef enum env_options
{
    ENV_ERROR            = -1,
    ENV_NO_OPTION        = 0x0000,
    ENV_NO_ENV           = 0x0001,
    ENV_PRINT_NO_NEWLINE = 0x0002,
    ENV_VERBOSE          = 0x0004,
    ENV_PRINT_HELP       = 0x0008,
} env_e;

typedef struct __attribute__((aligned(MEDIUM_ALIGNMENT_CONSTANT))) s_env
{
    char       **cpy;
    char       **save;
    const char  *change_dir;
    const char  *working_dir;
    const char **unset_env;
    const char **set_env;
    unsigned int option;
} t_env;

/*****************************************************************************
 * BUILTINS FUNCTIONS
 ******************************************************************************/
int         ft_env(const char **args, t_shell *shell);
void        ft_exit(const char **args, t_shell *shell);
int         ft_setenv(const char **args, t_shell *shell);
char      **ft_modify_env(const char *key, const char *value, char **env, char interpret);
int         ft_unsetenv(const char **args, t_shell *shell);
int         ft_cd(const char **args, t_shell *shell);
char       *cd_check(const char *dir, const char *cmd_name, t_shell *shell);
char       *cd_point(char **args, char ***env);
char       *cd_slash(char **args, char ***env);
void        cd_change_pwds(const char *pwd, t_shell *shell);
int         ft_echo(const char **args, t_shell *shell);
const char *ft_real_getenv(const char *str, const char **env);
const char *ft_getenv(const char *str, t_shell *shell);
char       *ft_getcwd(const char *dir, t_shell *shell);

#endif /* FT_BUILTINS_H */
