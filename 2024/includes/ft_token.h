/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_token.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2013/12/28 04:49:32 by gbourgeo          #+#    #+#             */
/*   Updated: 2018/04/06 17:34:07 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_TOKEN_H
#define FT_TOKEN_H

#include "ft_constants.h"
#include <stddef.h>

typedef enum token_type
{
    /* Type de token de base */
    TOKEN = 0,
    OPERATOR,
    NEWLINE,
    EXPANSION,
    /* Type de token spécifique */
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
{
    IS_A_QUOTE      = 0x01, // 2^0
    IS_AN_OPERATOR  = 0x02, // 2^1
    IS_A_COMMENT    = 0x04, // 2^2
    IS_AN_EXPANSION = 0x08, // 2^3
    IS_A_DELIMITER  = 0x10, // 2^4
    IS_A_SEPARATOR  = 0x20, // 2^5
} e_char_type;

#define CHARACTER_IS_A_QUOTE(value)      (((unsigned char *) &(value))[0] & (unsigned int) IS_A_QUOTE)
#define CHARACTER_IS_AN_OPERATOR(value)  (((unsigned char *) &(value))[0] & (unsigned int) IS_AN_OPERATOR)
#define CHARACTER_IS_A_COMMENT(value)    (((unsigned char *) &(value))[0] & (unsigned int) IS_A_COMMENT)
#define CHARACTER_IS_AN_EXPANSION(value) (((unsigned char *) &(value))[0] & (unsigned int) IS_AN_EXPANSION)
#define CHARACTER_IS_A_DELIMITER(value)  (((unsigned char *) &(value))[0] & (unsigned int) IS_A_DELIMITER)
#define CHARACTER_IS_A_SEPARATOR(value)  (((unsigned char *) &(value))[0] & (unsigned int) IS_A_SEPARATOR)
#define CHARACTER_IS_NEWLINE(value)      value == '\n'
#define CHARACTER_IS_QUOTED(value)       (((unsigned char *) &(value))[1] != 0)
#define QUOTE_VALUE(value)               (((unsigned char *) &(value))[1])

typedef struct __attribute((aligned(SOFT_ALIGNMENT_CONSTANT))) s_token
{
    e_token_type    type;
    size_t          tail;
    size_t          head;
    struct s_token *next;
} t_token;

typedef struct __attribute((aligned(SOFT_ALIGNMENT_CONSTANT))) s_args
{
    int            type;
    int            pipe;
    char         **args;
    struct s_args *next;
} t_args;

/**
 * @brief Découpe la commande passée en paramètre en une liste simplement chaînée de token.
 * @param token Token de départ
 * @param command Chaîne de caractère(s) à parser
 * @param ifs Chaîne de caractère(s) de séparation de mots
 * @return Liste de token.
 */
void        ft_token_recognition(t_token **token, const char *command, const char *ifs);

/**
 * @brief Désalloue une liste de token.
 * @param token_list Liste de token
 */
void        ft_free_token_list(t_token *token_list);

/**
 * @brief Retourne le type de token en chaîne de caractères.
 * @param type Type de token
 * @return Chaîne de caractère du type de token
 */
const char *ft_token_type_to_str(e_token_type type);

#endif /* FT_TOKEN_H */
