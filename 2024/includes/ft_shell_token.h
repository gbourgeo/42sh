/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_shell_token.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2013/12/28 04:49:32 by gbourgeo          #+#    #+#             */
/*   Updated: 2018/04/06 17:34:07 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _FT_SHELL_TOKEN_H_
#define _FT_SHELL_TOKEN_H_

#include "ft_defines.h"
#include <stddef.h>
#include <stdint.h>

typedef struct _align(32) s_args
{
    int            type;
    int            pipe;
    char         **args;
    struct s_args *next;
} t_args;

#endif /* _FT_SHELL_TOKEN_H_ */
