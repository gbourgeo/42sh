/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2013/12/28 04:49:32 by gbourgeo          #+#    #+#             */
/*   Updated: 2018/04/06 17:34:07 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_ENV_H
# define FT_ENV_H

typedef enum env_options
{
    ENV_ERROR = -1,
    ENV_NO_OPTION = 0x0000,
    ENV_NO_ENV = 0x0001,
    ENV_PRINT_NO_NEWLINE = 0x0002,
    ENV_VERBOSE = 0x0004,
    ENV_PRINT_HELP = 0x0008
} e_env;

typedef struct  s_env
{
    char * const *  cpy;
    char * const *  save;
    const char *    change_dir;
    const char *    working_dir;
    const char **   unset_env;
    const char **   set_env;
    e_env           option;
}               t_env;

#endif /* FT_ENV_H */
