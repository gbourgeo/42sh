/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2013/12/28 04:49:32 by gbourgeo          #+#    #+#             */
/*   Updated: 2018/04/06 17:34:07 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_ECHO_H
# define FT_ECHO_H

typedef enum echo_options
{
    ECHO_END_WITH_NEWLINE      = 0x0001,
    ECHO_INTERPRET_ESCAPE_CHAR = 0x0002,
    ECHO_BREAK                 = 0x1000
} echo_e;

#endif /* FT_ECHO_H */
