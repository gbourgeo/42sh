/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_termkeys.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2013/12/28 04:49:32 by gbourgeo          #+#    #+#             */
/*   Updated: 2018/04/06 17:34:07 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_TERMKEYS_H
#define FT_TERMKEYS_H

#include "ft_shell.h"

#include <stddef.h>

#define MAX_KEY_SIZE 16

/**
 * @brief Structure des combinaisons de touches de clavier gérées par le shell.
 * char[16]: Code hexadécimale de la combinaison de touche.
 * int: Longueur de la combinaison de touche.
 * void(*)(t_shell*): Fonction gérant la combinaison.
 */
typedef struct s_special_key
{
    char code[16];
    int  code_len;
    void (*handler)(t_shell *);
} t_key;

/*****************************************************************************
** KEYBOARD functions handlers
******************************************************************************/

/**
 * @brief Fonction d'analyse de caractères.
 * @param[in] buffer Ligne de commande
 * @param[in] len Longueur ligne de commande
 * @param[in] shell Structure interne du shell
 */
void ft_key_analyser(const char *buffer, ssize_t len, t_shell *shell);

/**
 * @brief Fonction de réinitialisation de la ligne de commande. Annule la
 * commande en cours et réaffiche un prompt vide.
 * @param[in] shell Structure interne du shell
 */
void ft_control_c(t_shell *shell);

/**
 * @brief Fonction d'arret du shell, si la ligne de commande est vide.
 * @param[in] shell Structure interne du shell
 */
void ft_control_d(t_shell *shell);
void ft_delete_character_left(t_shell *shell);
void ft_delete_character_right(t_shell *shell);
void ft_delete_word_left(t_shell *shell);
void ft_delete_word_right(t_shell *shell);

/**
 * @brief Fonction de démarrage/arret du mode de surlignage.
 * @param[in] shell Structure instern du shell
 */
void ft_highlight_mode(t_shell *shell);

/**
 * @brief Fonction de copie du/des textes surlignés
 * @param[in] shell Structure instern du shell
 */
void ft_highlight_paste(t_shell *shell);

/**
 * @brief Fonction de sauvegarde du/des textes surlignés.
 * @param[in] shell Structure instern du shell
 */
void ft_highlight_yank(t_shell *shell);
void ft_move_cursor_end_of_command(t_shell *shell);
void ft_move_cursor_start_of_commmand(t_shell *shell);
void ft_move_cursor_right(t_shell *shell);
void ft_move_cursor_left(t_shell *shell);
void ft_move_cursor_up(t_shell *shell);
void ft_move_cursor_down(t_shell *shell);
void ft_move_word_right(t_shell *shell);
void ft_move_word_left(t_shell *shell);
void ft_move_shift_right(t_shell *shell);
void ft_move_shift_left(t_shell *shell);
void ft_insert_character(char c, t_shell *shell);
void ft_print_character(t_shell *shell);
void ft_print_prev_command(t_shell *shell);
void ft_print_next_command(t_shell *shell);
void ft_print_command(t_shell *shell, char restore_pos);

#endif /* FT_TERMKEYS_H */
