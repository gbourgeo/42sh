/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_deletechar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/02/11 19:51:01 by gbourgeo          #+#    #+#             */
/*   Updated: 2014/02/11 19:55:48 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void        ft_deletechar(char *str, int pos)
{
    if (str == NULL || str[pos] == '\0')
        return ;
    while (str[pos])
    {
        str[pos] = str[pos + 1];
        pos++;
    }
}
