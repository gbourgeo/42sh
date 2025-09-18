/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_shell_command.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/01/20 12:12:34 by gbourgeo          #+#    #+#             */
/*   Updated: 2014/03/27 19:21:14 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _FT_SHELL_COMMAND_H_
#define _FT_SHELL_COMMAND_H_

#include "ft_defines.h"
#include "ft_shell_terminal.h"
#include <stddef.h>
#include <stdint.h>

typedef enum new_command_option_e
{
    SHELL_COMMAND_NEW_STEAL_LINE = 0,
    SHELL_COMMAND_NEW_DUPLICATE_LINE,
} e_newcmdopt;

typedef enum save_command_option_e
{
    SHELL_COMMAND_SAVE_FROM_HISTORIC = 0,
    SHELL_COMMAND_SAVE_FROM_INTERACTIVE_MODE,
} e_savecmdopt;

typedef enum command_operation_e
{
    COMMAND_REMOVE_CHAR_LEFT = 0,
    COMMAND_REMOVE_CHAR_RIGHT,
} e_cmdop;

typedef enum command_change_position_operation_e
{
    COMMAND_POS_ADD_VALUE = 0,
    COMMAND_POS_SUBSTRACT_VALUE,
} e_posop;

typedef enum highlight_area_operation_e
{
    SHELL_HIGHLIGHTED_AREA_ADD_CHAR = 0,
    SHELL_HIGHLIGHTED_AREA_REMOVE_CHAR_LEFT,
    SHELL_HIGHLIGHTED_AREA_REMOVE_CHAR_RIGHT,
    SHELL_HIGHLIGHTED_AREA_MOVE_LEFT,
    SHELL_HIGHLIGHTED_AREA_MOVE_RIGHT,
} e_highop;

typedef enum change_historic_value_e
{
    COMMAND_HIGHLIGHT_MODE    = 0x01, // 0000 0001
    COMMAND_HISTORIC_MODIFIED = 0x02, // 0000 0010
} histval_e;

enum command_print_option_e
{
    COMMAND_PRINT_FROM_START      = 0x01, // 0000 0001
    COMMAND_PRINT_FROM_POS        = 0x02, // 0000 0010
    COMMAND_PRINT_FROM_POS_LESS   = 0x04, // 0000 0100
    COMMAND_PRINT_SET_CURSOR_END  = 0x08, // 0000 1000
};

/**
 * Zone de commande surlignée.
 */
typedef struct _align(32) s_highlighted_area
{
    struct s_highlighted_area *prev; /* Texte surligné précédent */
    size_t                     tail; /* Position du premier caractère */
    size_t                     head; /* Position du dernier caractère */
    struct s_highlighted_area *next; /* Texte surligné suivant */
} t_higharea;

/**
 * Structure de commande.
 */
typedef struct _align(128) s_command
{
    const uint8_t    *origin; /* Buffer originel (celui de la liste) */
    uint8_t          *buffer; /* Buffer */
    size_t            len;    /* Longueur du buffer */
    size_t            size;   /* Taille du buffer */
    size_t            pos;    /* Position d'écriture dans le buffer */
    uint32_t          option; /* Options de la commande */
    t_higharea       *harea;  /* Zone(s) de texte surlignée(s) */
    struct s_command *prev;   /* Commande précédente */
    struct s_command *next;   /* Commande suivante */
} t_cmd;

/******************************************************************************
 * FONCTIONS UTILES POUR LA COMMANDE
 ******************************************************************************/

/**
 * @brief Supprime la liste de commande.
 * @param[in] command Structure de commande
 */
void ft_shell_command_delete_list(t_cmd *command_list);

/**
 * Retourne le premier élément de la liste de commandes.
 * @param command Liste de commande
 * @return Le premier élément de la liste.
 */
t_cmd *ft_shell_command_get_first(t_cmd *command_list);

/**
 * @brief Retourne le dernier élément de la liste de commandes.
 * @param historic Lisie de commande
 * @return Le dernier élément de la liste.
 */
t_cmd *ft_shell_command_get_last(t_cmd *command_list);

/**
 * @brief Change la position de la commande. Options valides:
 *
 * - COMMAND_POS_ADD_VALUE, ajoute la valeur à la position.
 *
 * - COMMAND_POS_SUBSTRACT_VALUE, déduit la valeur à la position.
 *
 * @param command Structure d'une commande
 * @param newval Valeur à ajouter/supprimer sur la position courante
 * @param operation Operation à effectuer
 * @param terminal Structure du terminal
 */
void ft_command_change_pos(t_cmd  *command, size_t newval, e_posop operation, t_term *terminal);

/**
 * @brief Fonction d'insertion d'un caractère dans le buffer de la commande
 * en cours à la dernière position connue dans la commande.
 * @param[in] command Structure d'une commande du shell
 * @param[in] charac Caractère à insérer dans la commande
 */
void ft_shell_command_insert_character(t_cmd *command, uint8_t charac);

/**
 * @brief Supprime un caractère à la position du buffer de la commande.
 * @param[in] command Structure d'une commande
 * @param[in] operation Operation de suppression
 * @param[in] size Taille de suppression
 */
void ft_shell_command_delete_character(t_cmd *command, e_cmdop operation, size_t size);

/**
 * @brief Fonction d'affichage de la commande sur la sortie standard depuis le début.
 *
 * Options disponibles (cumulables):
 *
 * - COMMAND_PRINT_FROM_START       Réécrit la commande depuis le début,
 *
 * - COMMAND_PRINT_FROM_POS         Réécrit la commande depuis la position dans la commande,
 *
 * - COMMAND_PRINT_FROM_POS_LESS    Réécrit la commande depuis la position - 1 dans la commande,
 *
 * - COMMAND_PRINT_SET_CURSOR_END   Positionne le curseur en fin de commande
 *
 * @param[in] command Structure de commande
 * @param[in] terminal Structure de terminal
 * @param[in] options Options d'affichage
 */
void ft_shell_command_print(t_cmd *command, t_term *terminal, uint32_t options);

/**
 * @brief Création d'une nouvelle commande avec vol ou duplication de la ligne
 * et insertion au début de la liste de commandes.
 * @param line Ligne à conserver
 * @param command_list Début de la liste de commandes
 * @param option Option de copie de la ligne
 * @return Nouvelle commande.
 */
t_cmd *ft_shell_command_new(const uint8_t *line,
                            t_cmd         *command_list,
                            e_newcmdopt    option);

/**
 * @brief Agrandit le buffer d'une commande, copie l'ancien buffer dans le nouveau
 * tronquant l'ancien si la taille est plus petite.
 * @param[in] command Structure d'une commande
 */
void ft_shell_command_realloc(t_cmd *command);

/**
 * @brief Réinitialise une commande.
 * @param[in] command Structure de commande
 */
t_cmd *ft_shell_command_reinit(t_cmd *command);

/**
 * Sauvegarde une nouvelle commande au début de la liste de commandes.
 *
 * Options disponibles:
 *
 * SHELL_COMMAND_SAVE_FROM_HISTORIC : La commande vient du fichier d'historique de commande.
 *
 * SHELL_COMMAND_SAVE_FROM_INTERACTIVE_MODE : La commande vient du mode intéractif du shell.
 * @param command Pointeur sur la liste de commandes
 * @param historic_size Nombre de commandes
 * @param historic_max_size Nombre maximum de commandes
 * @param option Option de traitement de la commande
 * @return La taille de la liste.
 */
size_t ft_shell_command_save(t_cmd      **command,
                             size_t       historic_size,
                             long         historic_max_size,
                             e_savecmdopt option);

/******************************************************************************
 * FONCTIONS UTILES POUR LES ZONES DE TEXTE SURLIGNE
 ******************************************************************************/

/**
 * @brief Déplace les valeurs de début et de fin d'une zone de texte surligné.
 *
 * Operation disponible:
 *
 * - SHELL_HIGHLIGHTED_AREA_ADD_CHAR            Un/Des caractères va être ajouté à la commande.
 *
 * - SHELL_HIGHLIGHTED_AREA_REMOVE_CHAR_LEFT    Un/Des caractères à gauche de la position va être supprimé de la commande.
 *
 * - SHELL_HIGHLIGHTED_AREA_REMOVE_CHAR_RIGHT   Un/Des caractères à droite de la position va être supprimé de la commande.
 *
 * - SHELL_HIGHLIGHTED_AREA_MOVE_LEFT           La position va être déplacé à gauche (utile lorsque le highlight mode est actif).
 *
 * - SHELL_HIGHLIGHTED_AREA_MOVE_RIGHT          La position va être déplacé à droite (utile lorsque le highlight mode est actif).
 *
 * @param command Structure d'une commande
 * @param operation Operation de décalage
 * @param value Valeur de l'opération
 */
void ft_command_highlight_move_areas(t_cmd *command, e_highop operation, size_t value);

/**
 * @brief Fonction de suppression de toutes les zones de texte surlignées d'une commande.
 * @param harea Liste des zones de texte surlignées
 */
t_higharea *ft_highlight_remove_all(t_higharea *harea);

/**
 * @brief Rewind de la liste des zones surlignées.
 * @param harea Liste des zones surlignées
 * @return Premier élément de la liste.
 */
t_higharea *ft_highlight_first(t_higharea *harea);

/**
 * @brief Fonction de réarrangement des zones surlignées.
 * La tete (head) et la queue (tail) sont réorganisées de telle sorte que la
 * tete soit toujours supérieure à la queue.
 * Les zones sont ensuite triées par ordre croissant, la première zone
 * surlignée du texte sera placée au début de la liste.
 * @param harea Liste des zones surlignées
 */
t_higharea *ft_highlight_sort_area(t_higharea *harea);

#endif /* _FT_SHELL_COMMAND_H_ */
