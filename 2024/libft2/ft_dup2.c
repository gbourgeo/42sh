/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dup2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/03/26 03:11:05 by gbourgeo          #+#    #+#             */
/*   Updated: 2018/04/06 17:35:25 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "libft2.h"

#include <unistd.h>

int ft_dup2(int oldd, int newd)
{
    if (newd != oldd)
        close(newd);
    return (dup2(oldd, newd));
}
