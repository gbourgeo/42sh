/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_command.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/01/20 12:12:34 by gbourgeo          #+#    #+#             */
/*   Updated: 2014/03/27 19:21:14 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_COMMAND_H
# define FT_COMMAND_H

# include <stddef.h>

# include "ft_history.h"

# define SHELL_BUFFER_SIZE  1024

typedef struct      s_command
{
    char            *buffer;        /* Commande */
    size_t          buffer_len;     /* Longueur de la commande */
    size_t          buffer_size;    /* Taille max de la commande */
    size_t          pos;            /* Position actuelle dans la commande */
    char            *original;      /* Buffer original sans historique */
}                   t_cmd;

void    ft_command_init(t_cmd *command);
void    ft_command_reinit(t_cmd *command);
void    ft_command_clear(t_cmd *command);
void    ft_command_replace(t_cmd *command, char *src, size_t src_len);
void    ft_command_realloc(t_cmd *command, size_t size);

#endif /* FT_COMMAND_H */
