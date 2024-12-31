/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_command.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/02/12 22:32:55 by gbourgeo          #+#    #+#             */
/*   Updated: 2014/03/27 18:50:55 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_command.h"

#include "libft.h"

#include <stdlib.h>

void ft_init_shell_command(t_cmd *command)
{
    ft_freestr(&command->buffer);
    command->buffer      = (char *) ft_memalloc(COMMAND_BUFFER_SIZE);
    command->buffer_size = COMMAND_BUFFER_SIZE;
    command->buffer_len  = 0;
    command->pos         = 0;
    ft_freestr(&command->original);
}

void ft_command_reinit(t_cmd *command)
{
    if (command->buffer_size > COMMAND_BUFFER_SIZE)
    {
        ft_init_shell_command(command);
    }
    else
    {
        ft_strclr(command->buffer);
        command->buffer_len = 0;
        command->pos        = 0;
        ft_freestr(&command->original);
    }
}

void ft_command_clear(t_cmd *command)
{
    ft_freestr(&command->buffer);
    command->buffer_size = 0;
    command->buffer_len  = 0;
    command->pos         = 0;
    ft_freestr(&command->original);
}

void ft_command_replace(t_cmd *command, char *src, size_t src_len)
{
    size_t size;

    size = command->buffer_size;
    /* Augmentation de la taille du buffer si nécessaire */
    if (src_len >= command->buffer_size)
    {
        ft_freestr(&command->buffer);
        size                 = COMMAND_BUFFER_SIZE * (src_len / COMMAND_BUFFER_SIZE + 1);
        command->buffer      = (char *) malloc(size);
        command->buffer_size = size;
    }
    /* Copie du buffer source */
    ft_strncpy(command->buffer, src, size);
    command->buffer_len = src_len;
    /* Positionne en début de buffer pour imprimer la commande */
    command->pos        = 0;
}

void ft_command_realloc(t_cmd *command, size_t size)
{
    char *save;

    save = command->buffer;
    /* La taille doit être un multiple de COMMAND_BUFFER_SIZE */
    if (size % COMMAND_BUFFER_SIZE != 0)
        return;
    command->buffer = (char *) malloc(size);
    ft_strncpy(command->buffer, save, size);
    command->buffer_size = size;
    free(save);
}
