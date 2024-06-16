/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/02/11 18:09:19 by gbourgeo          #+#    #+#             */
/*   Updated: 2014/03/13 16:18:12 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char        *ft_realloc(char *str, int size)
{
    char    *new;

    if ((new = (char *)ft_memalloc(size)) == NULL)
        return (NULL);
    if (str != NULL)
    {
        ft_strncpy(new, str, size - 1);
        free(str);
    }
    return (new);
}
