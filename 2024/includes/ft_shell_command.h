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

/******************************************************************************
 * ENUMERES ET STRUCTURE DES ZONES DE TEXTE SURLIGNE
 ******************************************************************************/

typedef enum highlight_area_operation_e
{
    SHELL_HIGHLIGHTED_AREA_ADD_CHAR = 0,
    SHELL_HIGHLIGHTED_AREA_REMOVE_CHAR_LEFT,
    SHELL_HIGHLIGHTED_AREA_REMOVE_CHAR_RIGHT,
    SHELL_HIGHLIGHTED_AREA_MOVE_LEFT,
    SHELL_HIGHLIGHTED_AREA_MOVE_RIGHT,
} e_highop;

typedef struct _align(32) s_highlighted_area
{
    struct s_highlighted_area *prev; /* Texte surligné précédent */
    size_t                     tail; /* Position du premier caractère */
    size_t                     head; /* Position du dernier caractère */
    struct s_highlighted_area *next; /* Texte surligné suivant */
} t_higharea;

/******************************************************************************
 * ENUMERES, DEFINITIONS ET STRUCTURE DES TOKENS
 ******************************************************************************/

typedef enum token_type
{
    /* Type de token de base */
    TOKEN = 0,
    OPERATOR,
    NEWLINE,
    EXPANSION,
    /* Type de token spécifique */
    WORD,
    OPERATOR_AMPERSAND,
    OPERATOR_PIPE,
    OPERATOR_SEMICOLON,
    OPERATOR_INPUT_REDIRECTION,
    OPERATOR_OUTPUT_REDIRECTION,
    OPERATOR_LEFT_PARENTHESIS,
    OPERATOR_RIGHT_PARENTHESIS,
    OPERATOR_AND,
    OPERATOR_OR,
    OPERATOR_DUPLICATE_INPUT_REDIRECTION,
    OPERATOR_DUPLICATE_OUTPUT_REDIRECTION,
    OPERATOR_READ_WRITE_REDIRECTION,
    OPERATOR_APPEND_OUTPUT_REDIRECTION,
    HERE_DOCUMENT,
    IO_NUMBER,
} e_token_type;

typedef enum char_type
{                           // Bits
    IS_A_QUOTE      = 0x01, // 00000001
    IS_AN_EXPANSION = 0x02, // 00000010
    IS_AN_OPERATOR  = 0x04, // 00000100
    IS_A_COMMENT    = 0x08, // 00001000
    IS_A_SEPARATOR  = 0x10, // 00010000
    IS_A_DELIMITER  = 0x20, // 00100000
} e_char_type;

#define CHARACTER_IS_A_QUOTE(value)      (((unsigned char *) (value))[0] & (unsigned int) IS_A_QUOTE)
#define CHARACTER_IS_AN_EXPANSION(value) (((unsigned char *) (value))[0] & (unsigned int) IS_AN_EXPANSION)
#define CHARACTER_IS_AN_OPERATOR(value)  (((unsigned char *) (value))[0] & (unsigned int) IS_AN_OPERATOR)
#define CHARACTER_IS_A_COMMENT(value)    (((unsigned char *) (value))[0] & (unsigned int) IS_A_COMMENT)
#define CHARACTER_IS_A_SEPARATOR(value)  (((unsigned char *) (value))[0] & (unsigned int) IS_A_SEPARATOR)
#define CHARACTER_IS_A_DELIMITER(value)  (((unsigned char *) (value))[0] & (unsigned int) IS_A_DELIMITER)
#define CHARACTER_IS_NEWLINE(value)      ((unsigned char) (value) == '\n')
#define CHARACTER_IS_QUOTED(value)       (((unsigned char *) (value))[1] != 0)
#define QUOTE_VALUE(value)               (((unsigned char *) (value))[1])

typedef struct _align(64) s_token
{
    e_token_type    type;         /* Type du token */
    size_t          tail;         /* Debut du token */
    size_t          head;         /* Fin du token */
    struct s_token *nested_token; /* Liste de tokens inclus dans celui-ci
                                     (ex. expansions imbriquées) */
    struct s_token *next;         /* Token suivant */
} t_token;

/******************************************************************************
 * ENUMERES ET STRUCTURE DES COMMANDES
 ******************************************************************************/

typedef enum new_command_option_e
{
    SHELL_COMMAND_NEW_STEAL_LINE = 0,
    SHELL_COMMAND_NEW_DUPLICATE_LINE,
} e_newcmdopt;

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

typedef enum command_parse_return_e
{
    SHELL_COMMAND_PARSED = 0,
    SHELL_COMMAND_PARSE_ABORTED,
} e_comparse;

typedef enum change_historic_value_e
{
    COMMAND_HIGHLIGHT_MODE    = 0x01, // 0000 0001
    COMMAND_HISTORIC_MODIFIED = 0x02, // 0000 0010
} histval_e;

typedef enum command_print_option_e
{
    COMMAND_PRINT_FROM_START    = 0x01, // 0000 0001
    COMMAND_PRINT_FROM_POS      = 0x02, // 0000 0010
    COMMAND_PRINT_FROM_POS_LESS = 0x04, // 0000 0100
} e_printopt;

typedef struct _align(128) s_command
{
    const uint8_t    *origin; /* Buffer originel (celui de la liste) */
    uint8_t          *buffer; /* Buffer */
    size_t            len;    /* Longueur du buffer */
    size_t            size;   /* Taille du buffer */
    size_t            pos;    /* Position d'écriture dans le buffer */
    uint32_t          option; /* Options */
    t_higharea       *harea;  /* Liste de Zone(s) de texte surlignée(s) */
    t_token          *token;  /* Liste de Token(s) (découpe de la commande) */
    struct s_command *prev;   /* Commande précédente */
    struct s_command *next;   /* Commande suivante */
} t_cmd;

/******************************************************************************
 * FONCTIONS UTILES DES COMMANDES
 ******************************************************************************/

/**
 * @brief Supprime la liste de commande.
 * @param[in] command Structure d'une Commande
 */
void ft_shell_command_delete_all(t_cmd *command);

/**
 * Retourne le premier élément de la liste de commandes.
 * @param command Liste de commande
 * @return Le premier élément de la liste.
 */
t_cmd *ft_shell_command_get_first(const t_cmd *command);

/**
 * @brief Retourne le dernier élément de la liste de commandes.
 * @param command_list Lisie de commande
 * @return Le dernier élément de la liste.
 */
t_cmd *ft_shell_command_get_last(const t_cmd *command_list);

/**
 * @brief Change la position de la commande. Options valides:
 *
 * - COMMAND_POS_ADD_VALUE, ajoute la valeur à la position.
 *
 * - COMMAND_POS_SUBSTRACT_VALUE, déduit la valeur à la position.
 *
 * @param command Structure d'une Commande
 * @param newval Valeur à ajouter/supprimer sur la position courante
 * @param operation Operation à effectuer
 * @param terminal Structure du terminal
 */
void ft_command_change_pos(t_cmd *command, size_t newval, e_posop operation, t_term *terminal);

/**
 * @brief Fonction d'insertion d'un caractère dans le buffer de la commande
 * en cours à la dernière position connue dans la commande.
 * @param[in] command Structure d'une Commande du shell
 * @param[in] charac Caractère à insérer dans la commande
 */
void ft_shell_command_insert_character(t_cmd *command, uint8_t charac);

/**
 * @brief Fonction de deboggage du Shell. Affiche les informations suivantes:
 *
 * - 1ère ligne: Ligne (actuelle/totale), Colonne (actuelle/totale), highlight
 * (activé/désactivé), Position du curseur dans la ligne de commande, Longueur
 * du Prompt.
 *
 * - 2nd ligne : Si non NULL, affiche les caractères du buffer passé en
 * paramètre au format hexadécimal.
 *
 * - 3ème ligne: Affiche le zones de textes surlignées.
 *
 * - 4ème ligne: Affiche la ligne de commande courante du Shell.
 *
 * - 5ème ligne: Affiche la ligne de commande sauvegardée.
 *
 * - 6ème ligne: Affiche l'historique de commandes sur une seule ligne.
 *
 * @param buffer Buffer de STDIN ou FILE
 * @param size Taille du buffer
 */
void ft_shell_command_debug(const uint8_t *buf, long size);

/**
 * @brief Supprime un caractère à la position du buffer de la commande.
 * @param[in] command Structure d'une Commande
 * @param[in] operation Operation de suppression
 * @param[in] size Taille de suppression
 */
void ft_shell_command_delete_character(t_cmd *command, e_cmdop operation, size_t size);

/**
 * @brief Fonction d'affichage de la commande sur la sortie standard depuis le début.
 *
 * Options disponibles (cumulables): COMMAND_PRINT_FROM_START, COMMAND_PRINT_FROM_POS,
 * COMMAND_PRINT_FROM_POS_LESS.
 *
 * @param[in] command Structure d'une Commande
 * @param[in] terminal Structure de terminal
 * @param[in] options Options d'affichage
 */
void ft_shell_command_print(const t_cmd *command, const t_term *terminal, e_printopt option);

/**
 * @brief Création d'une nouvelle commande avec vol ou duplication de la ligne
 * et insertion entre le prev et le next.
 * @param line Ligne à conserver
 * @param prev Commande précédente à la nouvelle commande
 * @param next Commande suivante à la nouvelle commande
 * @param option Option de copie de la ligne
 * @return Nouvelle commande.
 */
t_cmd *ft_shell_command_new(const uint8_t *line,
                            t_cmd         *prev,
                            t_cmd         *next,
                            e_newcmdopt    option);

/**
 * @brief Parsing de la commande en cours.
 * @param[in] command Structure d'une Commande
 * @param[in] ifs Caractère(s) de séparation de commande
 * @return SHELL_COMMAND_PARSED si la commande à été parsé avec succès,
 * SHELL_COMMAND_PARSE_ABORTED autrement.
 */
e_comparse ft_shell_command_parse(t_cmd *command, const char *ifs);

/**
 * @brief Agrandit le buffer d'une commande, copie l'ancien buffer dans le nouveau
 * tronquant l'ancien si la taille est plus petite.
 * @param[in] command Structure d'une Commande
 */
void ft_shell_command_realloc(t_cmd *command);

/**
 * @brief Réinitialise une commande.
 * @param[in] command Structure d'une Commande
 */
t_cmd *ft_shell_command_reinit(t_cmd *command);

/**
 * @brief Supprime la dernière commande de la liste
 * @param[in] command Structure d'une Commande
 */
void ft_shell_command_remove_last(t_cmd **command);

/**
 * Sauvegarde une nouvelle commande au début de la liste de commandes.
 * @param command Structure d'une Commande
 * @param historic_size Nombre de commandes
 * @param historic_max_size Nombre maximum de commandes
 * @return La taille de la liste.
 */
size_t ft_shell_command_save(t_cmd *command,
                             size_t historic_size,
                             long   historic_max_size);

/******************************************************************************
 * FONCTIONS UTILES DES ZONES DE TEXTE SURLIGNE
 ******************************************************************************/

/**
 * @brief Rewind de la liste des zones surlignées.
 * @param harea Liste des zones surlignées
 * @return Premier élément de la liste.
 */
t_higharea *ft_command_highlight_first(const t_higharea *harea);

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
 * @param command Structure d'une Commande
 * @param operation Operation de décalage
 * @param value Valeur de l'opération
 */
void ft_command_highlight_move_areas(t_cmd *command, e_highop operation, size_t value);

/**
 * @brief Fonction de suppression de toutes les zones de texte surlignées d'une commande.
 * @param harea Liste des zones de texte surlignées
 */
t_higharea *ft_command_highlight_remove_all(t_higharea *harea);

/**
 * @brief Fonction de réarrangement des zones surlignées.
 * La tete (head) et la queue (tail) sont réorganisées de telle sorte que la
 * tete soit toujours supérieure à la queue.
 * Les zones sont ensuite triées par ordre croissant, la première zone
 * surlignée du texte sera placée au début de la liste.
 * @param harea Liste des zones surlignées
 */
t_higharea *ft_command_highlight_sort_area(t_higharea *harea);

/******************************************************************************
 * FONCTIONS UTILES DES TOKENS
 ******************************************************************************/

/**
 * @brief Fonction de deboggage des tokens après parsing d'une Commande.
 * @param token Liste de Token
 * @param buffer Buffer tokenisé
 */
void ft_command_token_debug(const t_token *token, const uint8_t *buffer);

/**
 * @brief Découpe la commande passée en paramètre en une liste simplement chaînée de token.
 * @param[out] token Liste de Token de résultat
 * @param[in] buffer Buffer à parser
 * @param[in] end_of_input Caractère(s) de fin
 * @param[in] ifs Séparateur(s) de mots
 * @return La position du caractère dans la Commande responsable
 * de l'arrêt de la fonction.
 */
size_t ft_command_token_recognition(t_token             **token,
                                    const uint8_t        *buffer,
                                    const uint8_t        *end_of_input,
                                    const uint8_t * const ifs);

/**
 * @brief Désalloue une liste de token.
 * @param token Liste de token
 */
void ft_command_token_remove_all(t_token *token);

#endif /* _FT_SHELL_COMMAND_H_ */
