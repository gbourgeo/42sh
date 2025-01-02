/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_history.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/01/20 12:12:34 by gbourgeo          #+#    #+#             */
/*   Updated: 2014/03/27 19:21:14 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_HISTORY_H
#define FT_HISTORY_H

#include "ft_constants.h"
#include <stddef.h>

typedef struct __attribute__((aligned(SOFT_ALIGNMENT_CONSTANT))) s_history
{
    struct s_history *prev;        /* Pointeur vers la commande précédente */
    char             *command;     /* Commande */
    size_t            command_len; /* Longueur de la commande */
    struct s_history *next;        /* Pointeur vers la commande suivante */
} t_hist;

t_hist *ft_init_shell_history(char *history_file);
t_hist *ft_history_new(char *buffer, t_hist *hist_list);
void    ft_history_remove_all(t_hist *hist_list);
t_hist *ft_history_rewind(t_hist *hist_list);

#endif /* FT_HISTORY_H */
