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
#define FT_COMMAND_H

#include <stddef.h>

#define COMMAND_BUFFER_SIZE 1024

/**
 * Structure de commande.
 * - buffer         Pointeur sur la commande
 * - buffer_len     Nombre de caractère actuellement dans la commande
 * - buffer_size    Taille max de la commande
 * - pos            Position d'écriture dans la commande
 * - original       Commande originale sans historique
 */
typedef struct s_command
{
    char  *buffer;      /* Commande */
    size_t buffer_len;  /* Longueur de la commande */
    size_t buffer_size; /* Taille max de la commande */
    size_t pos;         /* Position actuelle dans la commande */
    char  *original;    /* Buffer original sans historique */
} t_cmd;

/**
 * @brief Initialise une structure de commande.
 * @param[in] command Structure de commande
 */
void ft_init_shell_command(t_cmd *command);

/**
 * @brief Réinitialise une structure de commande.
 * @param[in] command Structure de commande
 */
void ft_command_reinit(t_cmd *command);

/**
 * @brief Remise à zéro d'une structure de commande.
 * @param[in] command Structure de commande
 */
void ft_command_clear(t_cmd *command);

/**
 * @brief Remplace le buffer de la commande par la nouvelle source.
 * @param[in] command Structure de commande
 * @param[in] src Buffer source
 * @param[in] src_len Longueur du buffer source
 */
void ft_command_replace(t_cmd *command, char *src, size_t src_len);

/**
 * @brief Réalloue le buffer d'une commande, copie l'ancien buffer dans le nouveau
 * tronquant l'ancien si la taille est plus petite.
 * @param[in] command Structure de commande
 * @param[in] size Nouvelle taille à allouer (DOIT être un multiple de COMMAND_BUFFER_SIZE)
 */
void ft_command_realloc(t_cmd *command, size_t size);

#endif /* FT_COMMAND_H */
