/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_key_history.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/03/13 16:03:26 by gbourgeo          #+#    #+#             */
/*   Updated: 2018/04/06 18:39:13 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_command.h"
#include "ft_highlight.h"
#include "ft_history.h"
#include "ft_shell.h"
#include "ft_termios.h"
#include "ft_termkeys.h"
#include "libft.h"

/**
 * @brief Fonction d'affichage de la commande précédente.
 * @param[in] shell_ctx Environnement du shell
 */
void ft_print_prev_command(t_shell *shell)
{
    t_hist *hist = shell->history;

    if (hist)
    {
        /* Premier de l'historique ou il y a un historique précédent */
        if (shell->command.original == NULL || hist->next != NULL)
        {
            ft_term_clear_modes(&shell->terminal); /* Désactive tous les modes actifs */
            shell->highlighted.on = 0;
            ft_highlight_remove_all(&shell->highlighted.texts);
            ft_move_cursor_start_of_commmand(shell);
            ft_term_clear_cursor_and_under(&shell->terminal); /* Efface du curseur jusqu'à la fin de la ligne et celles en dessous */
            /* Sauvegarde du buffer originel */
            if (shell->command.original == NULL)
            {
                shell->command.original = ft_strdup(shell->command.buffer);
            }
            else if (hist->next != NULL)
            {
                shell->history = hist->next;
            }
            ft_command_replace(&shell->command, shell->history->command, shell->history->command_len);
            ft_print_command(shell, 0);
        }
    }
}

/**
 * @brief Fonction d'affichage de la commande suivante.
 * @param[in] shell_ctx Environnement du shell
 */
void ft_print_next_command(t_shell *shell)
{
    t_hist *hist = shell->history;

    if (hist)
    {
        /* Il y a un historique suivant ou le dernier historique */
        if (hist->prev != NULL || shell->command.original != NULL)
        {
            ft_term_clear_modes(&shell->terminal); /* Désactive tous les modes actifs */
            shell->highlighted.on = 0;
            ft_highlight_remove_all(&shell->highlighted.texts);
            ft_move_cursor_start_of_commmand(shell);
            ft_term_clear_cursor_and_under(&shell->terminal); /* Efface du curseur jusqu'à la fin de la ligne et celles en dessous */
            if (hist->prev != NULL)
            {
                shell->history = hist->prev;
                ft_command_replace(&shell->command, shell->history->command, shell->history->command_len);
            }
            else if (shell->command.original != NULL)
            {
                ft_command_replace(&shell->command, shell->command.original, ft_strlen(shell->command.original));
                /* Destruction de la sauvegarde du buffer d'origine */
                ft_freestr(&shell->command.original);
            }
            ft_print_command(shell, 0);
        }
    }
}
