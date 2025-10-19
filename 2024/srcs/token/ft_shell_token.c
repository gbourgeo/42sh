/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_shell_token.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/03/05 10:59:50 by gbourgeo          #+#    #+#             */
/*   Updated: 2014/03/30 00:31:44 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_defines.h"
#include "ft_shell_command.h"
#include "libft.h"
#include <stdint.h>
#include <stdlib.h>

/**
 * @brief Création d'un nouveau token.
 * @param type Type du nouveau token
 * @param pos Position de départ du nouveau token
 * @return Nouveau token.
 */
static t_token *ft_token_new(e_token_type token_type, size_t pos)
{
    t_token *token = (t_token *) malloc(sizeof(*token));

    if (token != NULL)
    {
        token->type = token_type;
        token->tail = pos;
        token->head = pos;
        token->next = NULL;
    }
    return (token);
}

/**
 * @brief Délimite la fin d'un Token et défini précisemment le type du Token.
 * @param token Pointeur sur le Token courant
 * @param buffer Buffer de Commande
 * @param pos Position de fin du token et courante du buffer
 * @return Pointeur sur le token suivant.
 */
static t_token **ft_token_end(t_token **token, const uint8_t *buffer, size_t pos)
{
    size_t iter = 0;

    if (token == NULL || *token == NULL)
    {
        return (token);
    }
    (*token)->head = pos;
    (*token)->next = NULL;
    iter           = (*token)->tail;
    /* SHELL GRAMMAR */
    if ((*token)->type == OPERATOR) /* Reconnaissance d'un OPERATOR */
    {
        if (buffer[iter] == '&')
        {
            (*token)->type = OPERATOR_AMPERSAND;
        }
        else if (buffer[iter] == '|')
        {
            (*token)->type = OPERATOR_PIPE;
        }
        else if (buffer[iter] == ';')
        {
            (*token)->type = OPERATOR_SEMICOLON;
        }
        else if (buffer[iter] == '<')
        {
            (*token)->type = OPERATOR_INPUT_REDIRECTION;
        }
        else if (buffer[iter] == '>')
        {
            (*token)->type = OPERATOR_OUTPUT_REDIRECTION;
        }
        else if (buffer[iter] == '(')
        {
            (*token)->type = OPERATOR_LEFT_PARENTHESIS;
        }
        else if (buffer[iter] == ')')
        {
            (*token)->type = OPERATOR_RIGHT_PARENTHESIS;
        }
    }
    else if ((*token)->type == TOKEN)
    {
        /* Reconnaissance d'un IO_NUMBER */
        while (iter < (*token)->head && ft_isdigit(buffer[(*token)->tail]))
        {
            iter++;
        }
        if (iter == (*token)->head && (buffer[pos] == '<' || buffer[pos] == '>'))
        {
            (*token)->type = IO_NUMBER;
        }
        /* Reconnaissasnce d'un mot réservé */
        else
        {
        }
    }
    /* Alias */
    return (&(*token)->next);
}

/**
 * @brief Vérifie si le caractère est spécial: une Quote, une Expansion,
 * un Opérateur, un Commentaire ou un Séparateur.
 * @param buffer Buffer de commande
 * @param ifs Séparateur(s) de commande
 * @return Le type de caractère.
 */
static unsigned char is_special(const uint8_t *buffer, const uint8_t *ifs)
{
    if (*buffer == '\\' || *buffer == '"' || *buffer == '\'')
    {
        return (IS_A_QUOTE);
    }
    if (*buffer == '$' || *buffer == '`')
    {
        return (IS_AN_EXPANSION);
    }
    if (*buffer == '&' || *buffer == '|' || *buffer == ';' || *buffer == '<'
        || *buffer == '>')
    {
        return (IS_AN_OPERATOR);
    }
    if (*buffer == '#' || *buffer == '/')
    {
        return (IS_A_COMMENT);
    }
    if (ifs != NULL)
    {
        while (*ifs)
        {
            if (*buffer == *ifs)
            {
                return (IS_A_SEPARATOR);
            }
            ifs++;
        }
    }
    return (0x00);
}

/**
 * @brief Vérifie si le caractère à la position courante du buffer de Commande
 * peut former un Token de type Opérateur.
 * @param token Token
 * @param buffer Buffer de commande
 * @param pos Position courante dans le buffer
 * @return 1 si le caractère peut former un Opérateur, 0 autrement.
 */
static char can_form_an_operator(t_token **token, const uint8_t *buffer, size_t pos)
{
    size_t iter = 0;
    size_t len  = 0;

    if (token == NULL || *token == NULL)
    {
        return (0);
    }
    iter = (*token)->tail;
    len  = pos - iter + 1;
    if (ft_memcmp(&buffer[iter], "&&", len) == 0)
    {
        (*token)->type = OPERATOR_AND;
        return (1);
    }
    if (ft_memcmp(&buffer[iter], "||", len) == 0)
    {
        (*token)->type = OPERATOR_OR;
        return (1);
    }
    if (ft_memcmp(&buffer[iter], "<&", len) == 0)
    {
        (*token)->type = OPERATOR_DUPLICATE_INPUT_REDIRECTION;
        return (1);
    }
    if (ft_memcmp(&buffer[iter], "<>", len) == 0)
    {
        (*token)->type = OPERATOR_READ_WRITE_REDIRECTION;
        return (1);
    }
    if (ft_memcmp(&buffer[iter], ">&", len) == 0)
    {
        (*token)->type = OPERATOR_DUPLICATE_OUTPUT_REDIRECTION;
        return (1);
    }
    if (ft_memcmp(&buffer[iter], ">|", len) == 0)
    {
        (*token)->type = OPERATOR_OUTPUT_REDIRECTION;
        return (1);
    }
    if (ft_memcmp(&buffer[iter], ">>", len) == 0)
    {
        (*token)->type = OPERATOR_APPEND_OUTPUT_REDIRECTION;
        return (1);
    }
    if (ft_memcmp(&buffer[iter], "<<-", len) == 0) // Permet de valider "<<" et "<<-"
    {
        (*token)->type = HERE_DOCUMENT;
        return (1);
    }
    return (0);
}

/**
 * Vérifie si le token est un Opérateur.
 * @param token Token courant
 * @return 1 si le Token est taggé comme Opérateur, 0 autrement.
 */
static int token_is_an_operator(const t_token **token)
{
    return (token != NULL && *token != NULL
            && ((*token)->type == OPERATOR
                || (*token)->type == OPERATOR_AMPERSAND
                || (*token)->type == OPERATOR_PIPE
                || (*token)->type == OPERATOR_SEMICOLON
                || (*token)->type == OPERATOR_INPUT_REDIRECTION
                || (*token)->type == OPERATOR_OUTPUT_REDIRECTION
                || (*token)->type == OPERATOR_LEFT_PARENTHESIS
                || (*token)->type == OPERATOR_RIGHT_PARENTHESIS
                || (*token)->type == OPERATOR_AND
                || (*token)->type == OPERATOR_OR
                || (*token)->type == OPERATOR_DUPLICATE_INPUT_REDIRECTION
                || (*token)->type == OPERATOR_DUPLICATE_OUTPUT_REDIRECTION
                || (*token)->type == OPERATOR_READ_WRITE_REDIRECTION
                || (*token)->type == OPERATOR_APPEND_OUTPUT_REDIRECTION
                || (*token)->type == HERE_DOCUMENT));
}

/**
 * @brief Vérifie si le Token est une nouvelle ligne
 * @param token Token courant
 * @return 1 si le Token est taggé comme NEWLINE, 0 autrement.
 */
static int token_is_a_newline(const t_token **token)
{
    return (token != NULL && *token != NULL && (*token)->type == NEWLINE);
}

/**
 * @brief Gère les caractères d'Expansion.
 * @param token Token courant
 * @param buffer Buffer de Commande
 * @param pos Position dans le buffer
 * @param ifs Séparateur(s) de commande
 * @return La position de fin de l'expansion si trouvée.
 */
static size_t can_form_an_expansion(t_token      **token,
                                    const uint8_t *buffer,
                                    size_t         pos,
                                    const uint8_t *ifs)
{
    if (*token == NULL)                        // Si le token courant est NULL...
    {
        *token = ft_token_new(EXPANSION, pos); // Création d'un nouveau token !
    }
    else
    {
        (*token)->type = EXPANSION;
    }
    if (buffer[pos] == '$')
    {
        if (buffer[pos + 1] == '{') // ${ : Parameter Expansion
        {
            pos += ft_command_token_recognition(&(*token)->nested_token,
                                                buffer + pos + 2,
                                                (uint8_t *) "}",
                                                ifs)
                + 1;
        }
        else if (buffer[pos + 1] == '(')
        {
            if (buffer[pos + 2] == '(') // $(( : Arithmetic Expansion
            {
                pos += ft_command_token_recognition(&(*token)->nested_token,
                                                    buffer + pos + 3,
                                                    (uint8_t *) "))",
                                                    ifs)
                    + 2;
            }
            else // $( : buffer Substitution
            {
                pos += ft_command_token_recognition(&(*token)->nested_token,
                                                    buffer + pos + 2,
                                                    (uint8_t *) ")",
                                                    ifs)
                    + 1;
            }
        }
    }
    else if (buffer[pos] == '`') // Backquotes : buffer Substitution
    {
        pos += ft_command_token_recognition(&(*token)->nested_token,
                                            buffer + pos + 1,
                                            (uint8_t *) "`",
                                            ifs);
    }
    return (pos);
}

/**
 * @brief Vérifie la fin d'une Commande.
 * @param buffer Buffer de Commande
 * @param end_of_input Caractère(s) de fin
 * @return 1 si la fin d'une Commande est détectée, 0 autrement.
 */
static int is_end_of_input(const uint8_t *buffer, const uint8_t *end_of_input)
{
    return (buffer[0] == '\0'
            || (end_of_input[0] != '\0'
                && ft_memcmp(buffer, end_of_input, ft_ustrlen(end_of_input)) == 0));
}

static size_t can_form_a_comment(t_token      **token,
                                 const uint8_t *buffer,
                                 size_t         pos)
{
    if (buffer[pos] == '#')
    {
        token = ft_token_end(token, buffer, pos); // Fin du token précédent !
        while (buffer[pos + 1] != '\0'
            && buffer[pos + 1] != '\n')
        {
            pos++;
        }
    }
    else if (buffer[pos] == '/')
    {
        if (buffer[pos + 1] == '/')
        {
            token = ft_token_end(token, buffer, pos); // Fin du token précédent !
            while (buffer[pos + 1] != '\0'
                && buffer[pos + 1] != '\n')
            {
                pos++;
            }
        }
        else if (buffer[pos + 1] == '*')
        {
            token = ft_token_end(token, buffer, pos); // Fin du token précédent !
            while (buffer[pos + 1] != '\0'
                && buffer[pos]     != '*'
                && buffer[pos + 1] != '/')
            {
                pos++;
            }
        }
    }
    return (pos);
}

size_t ft_command_token_recognition(
    t_token             **token,
    const uint8_t        *buffer,
    const uint8_t        *end_of_input,
    const uint8_t * const ifs)
{
    t_token     **current_token = token;
    size_t        pos           = 0;
    unsigned char character[2]  = { 0 };

    /**
     * POSIX Shell 2.6.5 Field Splitting
     *  2. If the value of IFS is null, no field splitting shall be performed.
     */
    if (ifs == NULL)
    {
        *token = ft_token_new(TOKEN, 0); // Création d'un unique token !
        pos = ft_ustrlen(buffer);
        (void) ft_token_end(current_token, buffer, pos); // Fin de token !
        return (pos);
    }

    /**
     * "character" fonctionne comme ceci:
     * 00000000   00000000   <- début
     * caractère     |||||__ bit indiquant que le caractère courant est une quote
     * de quote      ||||___ bit indiquant que le caractère courant est un opérateur
     * ou d'         |||____ bit indiquant que le caractère courant est un commentaire
     * expansion     ||_____ bit indiquant que le caractère courant est un séparateur de mot
     *               |______ bit indiquant que le caractère courant est un délimiteur d'expansion
     */

    while (buffer[pos] != '\0')
    {
        CLEAR_8BIT(character[0]);
        character[0] |= is_special(buffer + pos, ifs);

        if (is_end_of_input(buffer + pos, end_of_input)) // Caractère(s) de fin ...
        {
            if (!CHARACTER_IS_QUOTED(character))         // ET qu'ils ne sont pas quotés ...
            {
                pos += ft_ustrlen(end_of_input);         // Incrément de l'itérateur
                break;                                   // Fin !
            }
        }
        if (token_is_an_operator((const t_token **) current_token))         // Si le token courant est un opérateur ...
        {
            if ((CHARACTER_IS_AN_OPERATOR(character) || buffer[pos] == '-') // Si le caractère courant est un opérateur ou '-',
                && can_form_an_operator(current_token, buffer, pos))        // Et qu'ils peuvent former un opérateur ...
            {
                pos++;
                continue;                                                      // Caractère suivant.
            }
            current_token = ft_token_end(current_token, buffer, pos);          // Sinon fin de token !
        }
        if (CHARACTER_IS_A_QUOTE(character))                                   // Si le caractère courant est une quote ... ( \ ou " ou ' )
        {
            if (!CHARACTER_IS_QUOTED(character))                               // ET qu'il n'est pas quoté ...
            {
                QUOTE_VALUE(character) = (unsigned char) buffer[pos];          // Sauvegarde du caractère de quote.
                if (current_token == NULL || *current_token == NULL)           // Si le nouveau token est NULL ...
                {
                    current_token  = ft_token_end(current_token, buffer, pos); // Fin de token !
                    *current_token = ft_token_new(TOKEN, pos);                 // Création d'un nouveau token !
                }
            }
            else if (QUOTE_VALUE(character) == (unsigned char) buffer[pos]) // OU qu'il termine la quote existante,
            {
                QUOTE_VALUE(character) = 0;                                 // Reset de la quote sauvegardée.
            }
        }
        else if (CHARACTER_IS_AN_EXPANSION(character)) // Si le caractère courant est le début d'une expansion ... ( $ ou ` )
        {
            if (!CHARACTER_IS_QUOTED(character))       // ET qu'il n'est pas quoté ...
            {
                pos = can_form_an_expansion(current_token, buffer, pos, ifs);
            }
        }
        else if (CHARACTER_IS_AN_OPERATOR(character))                      // Si le caractère courant est un opérateur ...
        {
            if (!CHARACTER_IS_QUOTED(character))                           // ET qu'il n'est pas déjà quotée ...
            {
                current_token  = ft_token_end(current_token, buffer, pos); // Fin de token !
                *current_token = ft_token_new(OPERATOR, pos);              // Création d'un nouveau token !
            }
        }
        else if (CHARACTER_IS_A_SEPARATOR(character))                     // Si le caractère courant est un séparateur... (généralement un whitespace)
        {
            if (!CHARACTER_IS_QUOTED(character))                          // ET qu'il n'est pas quoté...
            {
                current_token = ft_token_end(current_token, buffer, pos); // Fin de token !
            }
        }
        else if (CHARACTER_IS_A_COMMENT(character)) // Si le caractère courant est le début d'un commentaire (#)...
        {
            if (!CHARACTER_IS_QUOTED(character))    // ET qu'il n'est pas quoté...
            {
                pos = can_form_a_comment(current_token, buffer, pos);
            }
        }
        else if (CHARACTER_IS_NEWLINE(buffer[pos]))                              // Si le caractère courant est une newline (\n)...
        {
            if (!CHARACTER_IS_QUOTED(character)                                // Qu'il n'est pas quoté...
                && !token_is_a_newline((const t_token **) current_token))      // Et que le token n'est pas de type NEWLINE ...
            {
                current_token  = ft_token_end(current_token, buffer, pos);     // Fin de token !
                *current_token = ft_token_new(NEWLINE, pos);                   // Création d'un nouveau token !
                current_token  = ft_token_end(current_token, buffer, pos + 1); // Fin de token !
            }
        }
        if (*current_token == NULL && !CHARACTER_IS_A_SEPARATOR(character)) // Si le token courant est NULL et pas un séparateur de mot ...
        {
            *current_token = ft_token_new(TOKEN, pos);                      // Création d'un nouveau token !
        }
        if (QUOTE_VALUE(character) == '\\' && pos > 0                       // Si le caractère de quote est '\' et que le
            && buffer[pos - 1] == '\\')                                     // caractère précédent était un '\'...
        {
            QUOTE_VALUE(character) = 0;                                     // Reset de la quote sauvegardée.
        }
        pos++;
    }

    (void) ft_token_end(current_token, buffer, pos); // Fin de token !

    return (pos);
}

void ft_command_token_remove_all(t_token *token)
{
    t_token *next = NULL;

    while (token != NULL)
    {
        next = token->next;
        ft_command_token_remove_all(token->nested_token);
        free(token);
        token = next;
    }
}
