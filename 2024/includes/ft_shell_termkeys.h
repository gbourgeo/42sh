/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_shell_termkeys.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2013/12/28 04:49:32 by gbourgeo          #+#    #+#             */
/*   Updated: 2018/04/06 17:34:07 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _FT_SHELL_TERMKEYS_H_
#define _FT_SHELL_TERMKEYS_H_

#include "ft_defines.h"
#include "ft_shell.h"
#include <stddef.h>
#include <stdint.h>

#define SHELL_KEYBOARD_KEYLEN 8

/**
 * @brief Structure des combinaisons de touches de clavier gérées par le shell.
 * char[16]: Code hexadécimale de la combinaison de touche.
 * int: Longueur de la combinaison de touche.
 * void(*)(t_shell*): Fonction gérant la combinaison.
 */
typedef struct _align(32) s_special_key
{
    char   code[SHELL_KEYBOARD_KEYLEN];
    size_t code_len;
    void   (*handler)(t_shell *);
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
void ft_key_analyser(const uint8_t *buffer, size_t len, t_shell *shell);

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

/**
 * @brief Fonction de suppression d'un caractère à droite ou sous le curseur
 * (dépendamment de si celui-ci est une barre ou un block)
 * @param[in] shell Structure d'environnement du shell
 */
void ft_delete_character_right(t_shell *shell);

/**
 * @brief Fonction de suppression d'un caractère à gauche du curseur
 * @param[in] shell Structure d'environnement du shell
 */
void ft_delete_character_left(t_shell *shell);

/**
 * @brief Fonction de suppression d'un mot à droite ou sous le curseur
 * (dépendamment de si celui-ci est une barre ou un block)
 * @param[in] shell Structure d'environnement du shell
 */
void ft_delete_word_right(t_shell *shell);

/**
 * @brief Fonction de suppression d'un mot à gauche du curseur
 * @param[in] shell Structure d'environnement du shell
 */
void ft_delete_word_left(t_shell *shell);

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

/**
 * Fonction de déplacement du curseur à la fin de la commande.
 * Elle prend en compte le mode actif ou non de surlignage.
 * @param[in] shell Structure d'environnement du shell
 */
void ft_move_cursor_end_of_command(t_shell *shell);

/**
 * Fonction de déplacement du curseur au début de la commande.
 * Elle prend en compte le mode actif ou non de surlignage.
 * @param[in] shell Structure d'environnement du shell
 */
void ft_move_cursor_start_of_commmand(t_shell *shell);

/**
 * Déplace le curseur et la position dans le buffer de commande d'un caractère
 * vers la droite. Elle prend en compte le mode de surlignage.
 * @param[in] shell Structure du shell
 */
void ft_move_cursor_right(t_shell *shell);

/**
 * Déplace le curseur et la position dans le buffer de commande d'un caractère
 * vers la gauche. Elle prend en compte le mode actif de surlignage.
 * @param[in] shell Structure du shell
 */
void ft_move_cursor_left(t_shell *shell);

/**
 * Déplace le curseur et la position dans le buffer de commande d'un caractère
 * vers le haut si la commande est sur plusieurs lignes. Elle prend en compte
 * le mode actif de surlignage.
 * @param[in] shell Structure du shell
 */
void ft_move_cursor_up(t_shell *shell);

/**
 * Déplace le curseur et la position dans le buffer de commande d'un caractère
 * vers le bas si la commande est sur plusieurs lignes. Elle prend en compte
 * le mode actif de surlignage.
 * @param[in] shell Structure du shell
 */
void ft_move_cursor_down(t_shell *shell);

/**
 * Fonction de déplacement du curseur d'un mot sur la droite.
 * Elle prend en compte le mode actif ou non de surlignage.
 * @param[in] shell_ctx Structure d'environnement du shell
 */
void ft_move_word_right(t_shell *shell);

/**
 * Fonction de déplacement du curseur d'un mot sur la gauche.
 * Elle prend en compte le mode actif ou non de surlignage.
 * @param[in] shell_ctx Structure d'environnement du shell
 */
void ft_move_word_left(t_shell *shell);
// void ft_move_shift_right(t_shell *shell);
// void ft_move_shift_left(t_shell *shell);

void ft_print_prev_command(t_shell *shell);
void ft_print_next_command(t_shell *shell);

/**
 * @brief Supprime tous les modes actifs et réécrit la ligne de commande.
 * @param shell Structure du shell
 */
void ft_clear_modes(t_shell *shell);

#endif /* _FT_SHELL_TERMKEYS_H_ */
