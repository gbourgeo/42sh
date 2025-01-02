/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_frees.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/02/12 22:56:22 by gbourgeo          #+#    #+#             */
/*   Updated: 2014/03/27 04:19:42 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_shell.h"
#include "ft_token.h"
#include "libft.h"
#include <stdlib.h>

void ft_free_list(t_args **list)
{
    if (*list)
    {
        if ((*list)->next != NULL)
        {
            ft_free_list(&(*list)->next);
        }
        (*list)->type = 0;
        (*list)->pipe = 0;
        if ((*list)->args)
        {
            ft_freetab(&(*list)->args);
        }
        (*list)->next = NULL;
        free(*list);
        *list = NULL;
    }
}
