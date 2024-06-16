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

#include "ft_termkeys.h"
#include "ft_minishell.h"

/**
 * @brief Fonction de suppression d'un caractère à droite ou sous le curseur
 * (dépendamment de si celui-ci est une barre ou un block)
 * @param[in] shell_ctx Structure d'environnement du shell
*/
void        ft_delete_character_right(void *shell_ctx)
{
    t_shell     *shell;
    t_high_t    *high;

    shell = (t_shell *)shell_ctx;
    high = shell->highlighted.texts;
    /* Cas spécial du caractère dans une zone surlignée */
    if (shell->highlighted.on == 0)
    {
        while (high)
        {
            if (shell->command.pos >= high->tail
            && shell->command.pos < high->head)
            {
                if (shell->command.pos == high->tail)
                    high->tail += 1;
                else
                    high->head -= 1;
                if (high->tail == high->head)

                break;
            }
            high = high->next;
        }
    }
    /* Suppression du caractère du buffer et réécriture de la commande à l'écran */
    if (shell->command.buffer[shell->command.pos] != '\0')
    {
        ft_deletechar(shell->command.buffer, shell->command.pos);
        ft_tputs("dc", shell);                  /* Supprime un caractère àla position du curseur */
        ft_print_command(shell, 1);
    }
}

/**
 * @brief Fonction de suppression d'un caractère à gauche du curseur
 * @param[in] shell_ctx Structure d'environnement du shell
*/
void        ft_delete_character_left(void *shell_ctx)
{
    t_shell *shell;

    shell = (t_shell *)shell_ctx;

    ft_move_cursor_left(shell);
    ft_delete_character_right(shell);
}

/**
 * @brief Fonction de suppression d'un mot à droite ou sous le curseur
 * (dépendamment de si celui-ci est une barre ou un block)
 * @param[in] shell_ctx Structure d'environnement du shell
*/
void        ft_delete_word_right(void *shell_ctx)
{
    t_shell *shell;
    char    *buffer;

    shell = (t_shell *)shell_ctx;
    buffer = shell->command.buffer;
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
void        ft_delete_word_left(void *shell_ctx)
{
    t_shell *shell;
    char    *buffer;

    shell = (t_shell *)shell_ctx;
    buffer = shell->command.buffer;
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
