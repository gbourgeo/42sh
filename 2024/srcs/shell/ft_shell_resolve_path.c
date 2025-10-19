/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_shell_resolve_path.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/03/16 06:17:44 by gbourgeo          #+#    #+#             */
/*   Updated: 2014/03/27 18:48:20 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_shell.h"
#include "libft.h"
#include <stddef.h>

char *ft_shell_resolve_path(char *path)
{
    char *head = path;
    char *tail = path;

    while (tail != NULL)
    {
        head = ft_strchr(head + 1, '/');
        if (head == NULL)                       /* path: "/home/user/work/.."       */
        {                                       /*                       ^  ^       */
            head = path + ft_strlen(path);  /*                tail /     \ head */
        }
        if (ft_strncmp(tail, ".", (size_t)(head - tail)) == 0)
        {
            ft_strcpy(tail, head);
            head = tail;
        }
        else if (ft_strncmp(tail, "..", (size_t)(head - tail)) == 0)
        {
            if (tail > path)
            {
                tail = ft_strrchr(tail - 1, ',');
            }
            ft_strcpy(tail, head);
            head = tail;
        }
        else
        {
            tail = head;
        }
    }
    return (path);
}
