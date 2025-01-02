/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_key_suppr.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/03/13 17:32:26 by gbourgeo          #+#    #+#             */
/*   Updated: 2018/04/06 18:39:26 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_command.h"
#include "ft_highlight.h"
#include "ft_shell.h"
#include "ft_termios.h"
#include "ft_termkeys.h"
#include "libft.h"

/**
 * @brief Supprime un caractère du buffer de la commande.
 * @param[in] command Structure d'une commande
 */
static void ft_delete_character(t_cmd *command)
{
    char         *buffer = command->buffer;
    unsigned long pos    = command->pos;

    if (buffer[pos] != '\0')
    {
        ft_strcpy(buffer + pos, buffer + pos + 1);
        command->buffer_len -= 1;
    }
}

/**
 * @brief Fonction de suppression d'un caractère à droite ou sous le curseur
 * (dépendamment de si celui-ci est une barre ou un block)
 * @param[in] shell_ctx Structure d'environnement du shell
 */
void ft_delete_character_right(t_shell *shell)
{
    t_htext *high = shell->highlighted.texts;

    /* Cas spécial du caractère dans une zone surlignée */
    if (shell->highlighted.on == 0)
    {
        while (high)
        {
            if (shell->command.pos >= high->tail
                && shell->command.pos < high->head)
            {
                if (shell->command.pos == high->tail)
                {
                    high->tail += 1;
                }
                else
                {
                    high->head -= 1;
                }
                if (high->tail == high->head)
                {
                    break;
                }
            }
            high = high->next;
        }
    }
    /* Suppression du caractère du buffer et réécriture de la commande à l'écran */
    if (shell->command.buffer[shell->command.pos] != '\0')
    {
        ft_delete_character(&shell->command);
        ft_term_remove_char(&shell->terminal); /* Supprime un caractère à la position du curseur */
        ft_print_command(shell, 1);
    }
}

/**
 * @brief Fonction de suppression d'un caractère à gauche du curseur
 * @param[in] shell_ctx Structure d'environnement du shell
 */
void ft_delete_character_left(t_shell *shell)
{
    if (shell->command.pos > 0)
    {
        ft_move_cursor_left(shell);
        ft_delete_character_right(shell);
    }
}

/**
 * @brief Fonction de suppression d'un mot à droite ou sous le curseur
 * (dépendamment de si celui-ci est une barre ou un block)
 * @param[in] shell_ctx Structure d'environnement du shell
 */
void ft_delete_word_right(t_shell *shell)
{
    char *buffer = shell->command.buffer;

    while (ft_iswhitespace(buffer[shell->command.pos]))
    {
        ft_delete_character_right(shell);
    }
    while (ft_isprint(buffer[shell->command.pos])
           && !ft_iswhitespace(buffer[shell->command.pos]))
    {
        ft_delete_character_right(shell);
    }
}

/**
 * @brief Fonction de suppression d'un mot à gauche du curseur
 * @param[in] shell_ctx Structure d'environnement du shell
 */
void ft_delete_word_left(t_shell *shell)
{
    char *buffer = shell->command.buffer;

    while (shell->command.pos > 0
           && ft_iswhitespace(buffer[shell->command.pos - 1]))
    {
        ft_delete_character_left(shell);
    }
    while (shell->command.pos > 0
           && ft_isprint(buffer[shell->command.pos - 1])
           && !ft_iswhitespace(buffer[shell->command.pos - 1]))
    {
        ft_delete_character_left(shell);
    }
}
