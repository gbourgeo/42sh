/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_shell_builtins.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/01/20 12:12:34 by gbourgeo          #+#    #+#             */
/*   Updated: 2014/03/27 19:21:14 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _FT_SHELL_BUILTINS_H_
#define _FT_SHELL_BUILTINS_H_

#include "ft_shell.h"
#include "ft_shell_environ.h"

typedef enum e_cd_options
{
    CD_RESOLVE_BEFORE    = 0x0001,
    CD_RESOLVE_AFTER     = 0X0002,
    CD_PRINT_PATH        = 0x0004,
} cdopt_e;

typedef enum echo_options
{
    ECHO_END_WITH_NEWLINE      = 0x0001,
    ECHO_INTERPRET_ESCAPE_CHAR = 0x0002,
} echo_e;

/*****************************************************************************
 * BUILTINS FUNCTIONS
 ******************************************************************************/

/**
 * @brief Builtin "env".
 * @param args Liste d'arguments fournis
 * @param shell Environnement du shell
 * @return -1 si erreur, 0 autrement.
 */
int         ft_env(const char **args, t_shell *shell);
void        ft_exit(const char **args, t_shell *shell);
int         ft_setenv(const char **args, t_shell *shell);
int         ft_unsetenv(const char **args, t_shell *shell);
int         ft_cd(const char **args, t_shell *shell);
char       *cd_point(char **args, char ***env);
char       *cd_slash(char **args, char ***env);
void        cd_change_pwds(const char *pwd, t_shell *shell);
int         ft_echo(const char **args, t_shell *shell);

#endif /* _FT_SHELL_BUILTINS_H_ */
