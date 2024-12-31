/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_highlight.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/03/13 16:03:26 by gbourgeo          #+#    #+#             */
/*   Updated: 2018/04/06 18:39:13 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_highlight.h"

#include <stdlib.h>

void ft_init_shell_highlight(t_hlight *hlight)
{
    hlight->on    = 0;
    hlight->texts = NULL;
    hlight->yank  = NULL;
}

t_htext *ft_highlight_get_area(size_t pos, t_htext *htexts)
{
    while (htexts)
    {
        if (pos >= htexts->tail && pos < htexts->head)
            break;
        htexts = htexts->next;
    }
    return (htexts);
}

t_htext *ft_highlight_remove_area(t_htext *htext)
{
    t_htext *ret;

    ret = NULL;
    if (htext != NULL)
    {
        if (htext->prev)
        {
            ret               = htext->prev;
            htext->prev->next = htext->next;
        }
        else
            ret = htext->next;
        if (htext->next)
            htext->next->prev = htext->prev;
        /* Désallocation de la zone de surlignage */
        free(htext);
    }
    return (ret);
}

void ft_highlight_remove_all(t_htext **htexts)
{
    while (*htexts != NULL)
        *htexts = ft_highlight_remove_area(*htexts);
}

t_htext *ft_highlight_sort_area(t_htext *text)
{
    t_htext *ptr;
    size_t   pos;

    if (text == NULL)
        return NULL;
    /* Réarrangement de tail et head */
    if (text->head < text->tail)
    {
        pos        = text->head;
        text->head = text->tail;
        text->tail = pos;
        ft_highlight_sort_area(text->next);
    }
    ptr = text->next;
    if (ptr == NULL)
        return (text);
    if (ptr->head < text->head)
    {
        pos        = ptr->head;
        ptr->head  = text->head;
        text->head = pos;
        pos        = ptr->tail;
        ptr->tail  = text->tail;
        text->tail = pos;
    }
    return (text);
}
