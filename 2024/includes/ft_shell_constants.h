/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_shell_constants.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2013/12/28 04:49:32 by gbourgeo          #+#    #+#             */
/*   Updated: 2018/04/06 17:34:07 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _FT_SHELL_CONSTANTS_H_
#define _FT_SHELL_CONSTANTS_H_

/* Codes de retour du shell */
typedef enum e_shell_retcodes
{
    SHELL_STATUS_OK      = 0,
    SHELL_STATUS_WARNING = 1,
    SHELL_STATUS_FATAL   = 127,
} retcode_e;

/* Options du shell */
enum e_shell_options
{
    SHELL_TERMATTR_LOADED  = 0x01, // 0000 0001
    SHELL_DEBUG_MODE       = 0x02, // 0000 0010
    SHELL_INTERACTIVE_MODE = 0x04, // 0000 0100
};

#endif /* _FT_SHELL_CONSTANTS_H_ */
