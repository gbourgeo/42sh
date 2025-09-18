/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_shell_terminal.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2013/12/28 04:49:32 by gbourgeo          #+#    #+#             */
/*   Updated: 2018/04/06 17:34:07 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _FT_SHELL_TERMINAL_H_
#define _FT_SHELL_TERMINAL_H_

#include "ft_defines.h"
#include <stddef.h>
#include <stdint.h>
#include <termios.h>

/**
 * @brief Positions du tableau des capabilités du terminal
 */
enum
{
    TERMCAP_CLEAR_LINE_AND_UNDER = 0,    // Efface la ligne et celles sous le curseur
    TERMCAP_CLEAR_FROM_CURSOR_AND_UNDER, // Efface du curseur jusqu'à la fin de la ligne et celles en dessous
    TERMCAP_MOVE_CURSOR,                 // Déplace le curseur (sur la colonne #1 et la ligne #2)
    TERMCAP_REMOVE_CHARACTER,            // Efface 1 caractère sous la position du curseur
    TERMCAP_INSERT_MODE_OFF,             // Désactive le mode insertion
    TERMCAP_INSERT_MODE_ON,              // Active le mode insertion
    TERMCAP_MOVE_CURSOR_LEFT,            // Bouge le curseur à gauche d'1 colonne
    TERMCAP_ALL_MODE_OFF,                // Désactive tous les modes actifs
    TERMCAP_HIGHLIGHT_MODE_ON,           // Active le mode surlignage
    TERMCAP_MOVE_CURSOR_DOWN,            // Déplace le curseur au début de la ligne suivante
    TERMCAP_TABLE_SIZE,                  // Taille de la table interne des termcaps
};

/**
 * @brief Options de la position du curseur
 */
enum e_terminal_cursor_position
{
    MOVE_CURSOR_START   = 0x01, // 0000 0001
    MOVE_CURSOR_CURRENT = 0x02, // 0000 0010
    MOVE_CURSOR_END     = 0x04, // 0000 0100
    CHANGE_CURRENT      = 0x08, // 0000 1000
};

typedef struct _align(8) s_cursor_position
{
    int line;
    int column;
} t_cursor_pos;

typedef struct _align(128) s_termcap
{
    int            fd;                               /* File Descriptor du terminal */
    struct termios ios;                              /* Caractéristiques du terminal à restorer en fin de programme */
    char           desc[2048];                       /* Description du terminal retourné par la fonction tgetent() */
    char          *capabilities[TERMCAP_TABLE_SIZE]; /* Capacités du terminal */
    int            max_column;                       /* Nombre de colonne max du terminal */
    int            max_line;                         /* Nombre de ligne max du terminal */
    t_cursor_pos   current;                          /* Position du curseur actuel */
    t_cursor_pos   start;                            /* Position du curseur du début de la commande */
    t_cursor_pos   end;                              /* Position du curseur de fin de la commande */
} t_term;

/**
 * @brief Initialise la structure Terminal.
 * @param terminal Structure interne Terminal
 */
void ft_shell_terminal_init(t_term *terminal);

/**
 * @brief Nettoie la structure Terminal.
 * @param terminal Structure interne Terminal
 * @param options Options du terminal
 */
void ft_shell_terminal_clear(t_term *terminal, unsigned int options);

/**
 * @brief Récupère les dimensions du terminal (lignes et colonnes).
 * @param terminal Structure interne Terminal
 */
void ft_shell_terminal_get_size(t_term *terminal);

/**
 * @brief Récupère la position du curseur dans le terminal.
 * @param terminal Structure interne Terminal
 * @param option Options de sauvegarde de la position
 */
void ft_shell_terminal_get_cursor_position(t_term *terminal, uint32_t options);

/**
 * @brief Récupère la position du curseur à la position courante de commande
 * dans le terminal.
 * @param terminal Structure interne Terminal
 * @param command_pos Position courante dans le buffer de commande
 */
void ft_shell_terminal_calc_current_command_position(t_term *terminal,
                                                     size_t  command_pos);

/**
 * @brief Récupère la position du curseur en fin de commande dans le terminal.
 * @param terminal Structure interne Terminal
 * @param command_len Longueur du buffer de commande
 */
void ft_shell_terminal_calc_end_command_position(t_term *terminal,
                                                 size_t  command_len);

/**
 * @brief Fonction de chargement de la base de donnée des capacités du terminal.
 * Vérification des possiblités du terminal pour que le shell puisse fonctionner
 * correctement. Cette fonction vérifie la présence des capabilités :
 *
 * - "cd", "ed" : Efface la ligne et celles sous le curseur
 *
 * - "ce", "el" : Efface du curseur jusqu'à la fin de la ligne et celles en dessous
 *
 * - "cm" : Déplace le curseur du terminal
 *
 * - "dc", "dch1" : Efface 1 caractère sous la position du curseur
 *
 * - "ei", "rmir" : Désactive le mode insertion
 *
 * - "im", "smir" : Active le mode insertion
 *
 * - "le", "cub1" : Bouge le curseur à gauche d'1 colonne
 *
 * - "me", "sgr0" : Désactive tous les modes actifs
 *
 * - "mr", "rev" : Active le mode surlignage (reverse video)
 *
 * - "nw", "nel" : Déplace le curseur au début de la ligne suivante
 * @param shell     Structure interne du shell
 * @return 0 OK, 1 autrement.
 */
int ft_shell_terminal_load_termcaps(t_term *terminal, void *shell_ctx);

/**
 * @brief Fonction de chargement des nouveaux attributs du terminal.
 * @param shell Structure interne du shell
 * @return 0 OK, 1 autrement.
 */
int ft_shell_terminal_change_attributes(t_term *terminal);

/**
 * @brief Fonction de restoration des attributs du terminal.
 * @param shell Structure interne du shell
 */
void ft_shell_terminal_restore_attributes(t_term *terminal);

/**
 * @brief Efface la ligne et celles sous le curseur
 * @param shell Structure interne du shell
 */
void ft_term_clear_line_and_under(t_term *terminal);

/**
 * @brief Efface du curseur jusqu'à la fin de la ligne et celles en dessous.
 * @param shell Structure interne du shell
 */
void ft_term_clear_cursor_and_under(t_term *terminal);

/**
 * @brief Déplace le curseur du terminal suivant une position définie.
 * @param shell Structure interne du shell
 * @param options Options de déplacement (cf. e_terminal_cursor_position)
 */
void ft_term_move_cursor(t_term *terminal, uint32_t options);

/**
 * @brief Efface 1 caractère sous la position du curseur.
 * @param shell Structure interne du shell
 */
void ft_term_remove_char(t_term *terminal);

/**
 * @brief Désactive le mode insertion.
 * @param shell Structure interne du shell
 */
void ft_term_insert_mode_off(t_term *terminal);

/**
 * @brief Active le mode insertion.
 * @param shell Structure interne du shell
 */
void ft_term_insert_mode_on(t_term *terminal);

/**
 * @brief Désactive tous les modes actifs.
 * @param shell Structure interne du shell
 */
void ft_term_clear_modes(t_term *terminal);

/**
 * @brief Active le mode surlignage (reverse video).
 * @param shell Structure interne du shell
 */
void ft_term_highlight_mode_on(t_term *terminal);

/**
 * @brief Déplace le curseur au début de la ligne suivante.
 * @param shell Structure interne du shell
 */
void ft_term_move_cursor_down(t_term *terminal);

#endif /* _FT_SHELL_TERMINAL_H_ */
