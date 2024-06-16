/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_key_control.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/03/13 15:58:56 by gbourgeo          #+#    #+#             */
/*   Updated: 2018/04/06 18:38:49 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_termkeys.h"
#include "ft_minishell.h"

/**
 * @brief Fonction de réinitialisation de la ligne de commande. Annule la
 * commande en cours et réaffiche un prompt vide.
 * @param[in] shell_ctx Structure d'environnement du shell
*/
void        ft_control_c(void *shell_ctx)
{
    t_shell *shell;

    shell = (t_shell *)shell_ctx;
    ft_tputs("me", shell); /* Désactive tous les modes actifs */
    shell->highlighted.on = 0;
    ft_highlight_remove_all(&shell->highlighted.texts);
    write(STDOUT_FILENO, "^C", 2);
    ft_move_cursor_end_of_command(shell);
    ft_tputs("nw", shell); /* Déplace le curseur au début de la ligne suivante */
    ft_command_reinit(&shell->command);
    shell->prompt.print = 1;
}

/**
 * @brief Fonction d'arret du shell, si la ligne de commande est vide.
 * @param[in] shell_ctx Structure d'environnement du shell
*/
void        ft_control_d(void *shell_ctx)
{
    t_shell *shell;

    shell = (t_shell *)shell_ctx;
    if (shell->command.buffer_len == 0)
        ft_exit(NULL, shell);
}
