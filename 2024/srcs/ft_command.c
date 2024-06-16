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

#include "libft.h"
#include "ft_command.h"

/**
 * Initialise une commande.
 * @param[in] command Contexte d'une commande
 */
void    ft_command_init(t_cmd *command)
{
    ft_freestr(&command->buffer);
    command->buffer = (char *)ft_memalloc(SHELL_BUFFER_SIZE);
    command->buffer_size = SHELL_BUFFER_SIZE;
    command->buffer_len = 0;
    command->pos = 0;
    ft_freestr(&command->original);
}

/**
 * @brief Sauvegarde d'une commande dans la liste d'historique de commandes.
 * @param[in] command Contexte d'une commande
*/
void    ft_command_reinit(t_cmd *command)
{
    if (command->buffer_size > SHELL_BUFFER_SIZE)
    {
        ft_command_init(command);
    }
    else
    {
        ft_strclr(command->buffer);
        command->buffer_len = 0;
        command->pos = 0;
        ft_freestr(&command->original);
    }
}

/**
 * Nettoyage d'une commande
 * @param[in] command Contexte d'une commande
 */
void    ft_command_clear(t_cmd *command)
{
    ft_freestr(&command->buffer);
    command->buffer_size = 0;
    command->buffer_len = 0;
    command->pos = 0;
    ft_freestr(&command->original);
}

/**
 * Remplace le buffer de la commande par le nouveau buffer.
 * @param[in] command Contexte d'une commande
 * @param[in] src Buffer source
 * @param[in] src_len Longueur du buffer source
 */
void    ft_command_replace(t_cmd *command, char *src, size_t src_len)
{
    size_t  size;

    size = command->buffer_size;
    /* Augmentation de la taille du buffer si nécessaire */
    if (src_len >= command->buffer_size)
    {
        ft_freestr(&command->buffer);
        size = SHELL_BUFFER_SIZE * (src_len / SHELL_BUFFER_SIZE + 1);
        command->buffer = (char *)malloc(size);
        command->buffer_size = size;
    }
    /* Copie du buffer source */
    ft_strncpy(command->buffer, src, size);
    command->buffer_len = src_len;
    /* Positionne en début de buffer */
    command->pos = 0;
}

/**
 * Réalloue le buffer d'une commande, copie l'ancien buffer dans le nouveau
 * tronquant l'ancien si la taille est plus petite.
 * @param[in] command Contexte d'une commande
 * @param[in] size Nouvelle taille à allouer
 */
void    ft_command_realloc(t_cmd *command, size_t size)
{
    char    *save;

    save = command->buffer;
    /* La taille doit être un multiple de SHELL_BUFFER_SIZE */
    if (size % SHELL_BUFFER_SIZE != 0)
        return ;

    command->buffer = (char *)malloc(size);
    ft_strncpy(command->buffer, save, size);
    command->buffer_size = size;
    free(save);
}
