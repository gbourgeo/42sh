/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_shell_token.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2013/12/28 04:49:32 by gbourgeo          #+#    #+#             */
/*   Updated: 2018/04/06 17:34:07 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _FT_SHELL_TOKEN_H_
#define _FT_SHELL_TOKEN_H_

#include "ft_defines.h"
#include <stddef.h>
#include <stdint.h>

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
    struct s_token *nested_token; /* Liste de tokens inclus dans celui-ci (ex. expansions imbriquées) */
    struct s_token *next;         /* Token suivant */
} t_token;

typedef struct _align(32) s_args
{
    int            type;
    int            pipe;
    char         **args;
    struct s_args *next;
} t_args;

/**
 * @brief Fonction de deboggage des tokens après parsing d'une Commande.
 * @param token Liste de token
 */
void ft_shell_token_debug(const t_token *token);

/**
 * @brief Désalloue une liste de token.
 * @param token Liste de token
 */
void ft_shell_token_delete(t_token *token);

/**
 * @brief Découpe la commande passée en paramètre en une liste simplement chaînée de token.
 * @param[out] token Liste de Token de résultat
 * @param[in] buffer Buffer à parser
 * @param[in] end_of_input Caractère(s) de fin
 * @param[in] ifs Séparateur(s) de mots
 * @return Liste de token.
 */
size_t ft_shell_token_recognition(t_token      **token,
                                  const uint8_t *buffer,
                                  const uint8_t *end_of_input,
                                  const uint8_t *ifs);

/**
 * @brief Retourne le type de token en chaîne de caractères.
 * @param type Type de token
 * @return Chaîne de caractère du type de token
 */
const char *ft_token_type_to_str(e_token_type type);

#endif /* _FT_SHELL_TOKEN_H_ */
