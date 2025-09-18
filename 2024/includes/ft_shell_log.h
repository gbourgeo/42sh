/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_shell_log.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2013/12/28 04:49:32 by gbourgeo          #+#    #+#             */
/*   Updated: 2018/04/06 17:34:07 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _FT_SHELL_LOG_H_
#define _FT_SHELL_LOG_H_

typedef enum
{
    SH_LOG_LEVEL_DBG = 0,
    SH_LOG_LEVEL_INFO,
    SH_LOG_LEVEL_WARN,
    SH_LOG_LEVEL_ERR,
    SH_LOG_LEVEL_FATAL,
} log_level_e;

/*****************************************************************************
 * LOG FUNCTIONS
 ******************************************************************************/
void ft_log(log_level_e log_level, const char *err_str, ...);

#endif /* _FT_SHELL_LOG_H_ */
