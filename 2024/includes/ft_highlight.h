/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_highlight.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/03/13 16:03:26 by gbourgeo          #+#    #+#             */
/*   Updated: 2018/04/06 18:39:13 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_HIGHLIGHT_H
# define FT_HIGHLIGHT_H

#include <stddef.h>

typedef struct      s_highlighted_text
{
    size_t                      head;   /* Dernier caractère surligné de la commande */
    size_t                      tail;   /* Premier caractère surligné de la commande */
    struct s_highlighted_text   *next;  /* Texte surligné suivant */
    struct s_highlighted_text   *prev;  /* Texte surligné précédent */
}                   t_high_t;

typedef struct      s_highlight
{
    char            on;             /* Indique si le mode de surlignage est actif ou non */
    t_high_t        *texts;         /* Liste simplement chainé de textes surlignés */
    char            *yank;          /* Sauvegarde du texte surligné */
}                   t_high;

t_high_t        *ft_highlight_new_area(size_t pos, t_high_t *htexts);
t_high_t        *ft_highlight_get_area(size_t pos, t_high_t *htexts);
t_high_t        *ft_highlight_sort_area(t_high_t *htext);
t_high_t        *ft_highlight_remove_area(t_high_t *htext);
void            ft_highlight_remove_all(t_high_t **htexts);
void            ft_highlight_mode(void *shell_ctx);
void            ft_highlight_paste(void *shell_ctx);
void            ft_highlight_yank(void *shell_ctx);

#endif /* FT_HIGHLIGHT_H */
