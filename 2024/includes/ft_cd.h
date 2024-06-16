/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2013/12/28 04:49:32 by gbourgeo          #+#    #+#             */
/*   Updated: 2018/04/06 17:34:07 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_CD_H
# define FT_CD_H

typedef enum cd_options
{
    CD_RESOLVE_UNDEFINED = 0,
    CD_RESOLVE_BEFORE = 0x0001,
    CD_RESOLVE_AFTER = 0X0002,
    CD_BREAK = 0x1000
} cd_e;

#endif /* FT_CD_H */
