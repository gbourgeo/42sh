/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_argsnew.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/03/26 08:55:50 by gbourgeo          #+#    #+#             */
/*   Updated: 2014/03/27 00:03:08 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_token.h"
#include "libft.h"
#include <stdlib.h>

t_args *ft_argsnew(const char **args, int type)
{
    t_args *new = NULL;

    if ((new = (t_args *) malloc(sizeof(t_args))) == NULL)
    {
        return (NULL);
    }
    new->type = type;
    new->pipe = 0;
    new->args = ft_tabdup(args);
    new->next = NULL;
    return (new);
}
