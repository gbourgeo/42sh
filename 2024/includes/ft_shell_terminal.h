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
 * @brief Options de déplacement du curseur
 */
typedef enum e_terminal_cursor_position
{
    MOVE_CURSOR_START   = 1,
    MOVE_CURSOR_CURRENT = 2,
    MOVE_CURSOR_END     = 3,
    // CHANGE_CURRENT      = 0x08, // 0000 1000
} e_cursormove;

typedef enum e_terminal_cursor_set
{
    SET_CURSOR_CURRENT = 0,
    SET_CURSOR_ALL     = 1,
} e_cursorset;

typedef struct _align(8) s_cursor_position
{
    int line;
    int column;
} t_cursor_pos;

typedef struct _align(128) s_termcap
{
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
 * @brief Récupère les dimensions du terminal (lignes et colonnes).
 * @param terminal Structure interne Terminal
 */
void ft_shell_terminal_get_size(t_term *terminal);

/**
 * @brief Sauvegarde la position du curseur dans le terminal.
 *
 * Option disponible: SET_CURSOR_CURRENT, SET_CURSOR_ALL.
 *
 * @param terminal Structure interne Terminal
 * @param option Option de sauvegarde de la position
 */
void ft_shell_terminal_get_cursor_position(t_term *terminal, e_cursorset option);

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
 * @param terminal Structure interne Terminal
 * @param term Nom du terminal (variable d'environnement TERM)
 * @return 0 OK, 1 autrement.
 */
int ft_shell_terminal_load(t_term *terminal, const char *term);

/**
 * @brief Efface la ligne et celles sous le curseur
 * @param terminal Structure interne Terminal
 */
void ft_term_clear_line_and_under(const t_term *terminal);

/**
 * @brief Efface du curseur jusqu'à la fin de la ligne et celles en dessous.
 * @param terminal Structure interne Terminal
 */
void ft_term_clear_cursor_and_under(const t_term *terminal);

/**
 * @brief Déplace le curseur du terminal suivant une position définie.
 *
 * Position possible: MOVE_CURSOR_START, MOVE_CURSOR_CURRENT, MOVE_CURSOR_END.
 *
 * @param terminal Structure interne Terminal
 * @param move Options de déplacement (cf. e_terminal_cursor_position)
 */
void ft_term_move_cursor(const t_term *terminal, e_cursormove move);

/**
 * @brief Efface 1 caractère sous la position du curseur.
 * @param terminal Structure interne Terminal
 */
void ft_term_remove_char(const t_term *terminal);

/**
 * @brief Désactive le mode insertion.
 * @param terminal Structure interne Terminal
 */
void ft_term_insert_mode_off(const t_term *terminal);

/**
 * @brief Active le mode insertion.
 * @param terminal Structure interne Terminal
 */
void ft_term_insert_mode_on(const t_term *terminal);

/**
 * @brief Désactive tous les modes actifs.
 * @param terminal Structure interne Terminal
 */
void ft_term_clear_modes(const t_term *terminal);

/**
 * @brief Active le mode surlignage (reverse video).
 * @param terminal Structure interne Terminal
 */
void ft_term_highlight_mode_on(const t_term *terminal);

/**
 * @brief Déplace le curseur au début de la ligne suivante.
 * @param terminal Structure interne Terminal
 */
void ft_term_move_cursor_down(const t_term *terminal);

#endif /* _FT_SHELL_TERMINAL_H_ */
