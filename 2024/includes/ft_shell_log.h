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

#include <stdint.h>

typedef enum
{
    SHELL_LOG_LEVEL_DBG = 0,
    SHELL_LOG_LEVEL_INFO,
    SHELL_LOG_LEVEL_WARN,
    SHELL_LOG_LEVEL_ERR,
    SHELL_LOG_LEVEL_FATAL,
} log_level_e;

#define SH_LOG_LEVEL_DBG    SHELL_LOG_LEVEL_DBG, __FILE__, __LINE__
#define SH_LOG_LEVEL_INFO   SHELL_LOG_LEVEL_INFO, __FILE__, __LINE__
#define SH_LOG_LEVEL_WARN   SHELL_LOG_LEVEL_WARN, __FILE__, __LINE__
#define SH_LOG_LEVEL_ERR    SHELL_LOG_LEVEL_ERR, __FILE__, __LINE__
#define SH_LOG_LEVEL_FATAL  SHELL_LOG_LEVEL_FATAL, __FILE__, __LINE__

/*****************************************************************************
 * LOG FUNCTIONS
 ******************************************************************************/

 /**
  * @brief Fonction de Log du Shell.
  * @param log_level    Niveau de log
  * @param file         Fichier d'appel
  * @param line         Ligne du fichier
  * @param err_str      Description de l'erreur et ses arguments
  */
void ft_shell_log(log_level_e log_level, const char *file, uint32_t line, const char *err_str, ...);

 /**
  * @brief Fonction de retour d'erreur système.
  * @return L'erreur système sous format char*.
  */
char *ft_shell_strerror(void);

#endif /* _FT_SHELL_LOG_H_ */
