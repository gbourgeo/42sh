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
# define FT_BUILTINS_H

/*
** BUILTINS
*/

void                ft_exit(const char **args, void *e);
int                 ft_env(const char **args, void *e);
int                 ft_setenv(const char **args, void *e);
int                 ft_unsetenv(const char **args, void *e);
int                 ft_cd(const char **args, void *e);
char                *cd_check(const char *dir, const char *cmd_name, void *e);
char                *cd_point(char **args, char ***env);
char                *cd_slash(char **args, char ***env);
void                cd_change_pwds(const char *pwd, void *e);
int                 ft_echo(const char **args, void *e);
const char          *ft_getenv(const char *str, char * const *env);
char                *ft_getcwd(const char *dir, void *e);



#endif /* FT_BUILTINS_H */
