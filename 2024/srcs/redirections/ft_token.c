/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_token.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/03/05 10:59:50 by gbourgeo          #+#    #+#             */
/*   Updated: 2014/03/30 00:31:44 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_defines.h"
#include "ft_shell_token.h"
#include "libft.h"
#include <stdint.h>
#include <stdlib.h>

/**
 * @brief Création d'un nouveau token.
 * @param type Type du nouveau token
 * @param pos Position de départ du nouveau token
 * @return Nouveau token.
 */
static t_token *ft_token_new(e_token_type type, size_t pos)
{
    t_token *token = (t_token *) malloc(sizeof(*token));

    if (token != NULL)
    {
        token->type = type;
        token->tail = pos;
        token->head = pos;
        token->next = NULL;
    }
    return (token);
}

/**
 * @brief Fin d'un token.
 * @param token Pointeur sur le token en cours
 * @param head Position de fin du token
 * @param command Commande
 * @return Pointeur sur le token suivant.
 */
static t_token **ft_token_end(t_token **token, size_t head, const uint8_t *command)
{
    size_t iter = 0;

    if (token == NULL || *token == NULL)
    {
        return (token);
    }
    (*token)->head = head;
    (*token)->next = NULL;
    iter           = (*token)->tail;
    /* Reconnaissance d'un IO_NUMBER */
    if ((*token)->type == TOKEN)
    {
        while (iter < (*token)->head && ft_isdigit(command[(*token)->tail]))
        {
            iter++;
        }
        if (iter == (*token)->head && (command[head] == '<' || command[head] == '>'))
        {
            (*token)->type = IO_NUMBER;
        }
    }
    /* Reconnaissance d'un OPERATOR */
    else if ((*token)->type == OPERATOR)
    {
        if (command[iter] == '&')
        {
            (*token)->type = OPERATOR_AMPERSAND;
        }
        else if (command[iter] == '|')
        {
            (*token)->type = OPERATOR_PIPE;
        }
        else if (command[iter] == ';')
        {
            (*token)->type = OPERATOR_SEMICOLON;
        }
        else if (command[iter] == '<')
        {
            (*token)->type = OPERATOR_INPUT_REDIRECTION;
        }
        else if (command[iter] == '>')
        {
            (*token)->type = OPERATOR_OUTPUT_REDIRECTION;
        }
        else if (command[iter] == '(')
        {
            (*token)->type = OPERATOR_LEFT_PARENTHESIS;
        }
        else if (command[iter] == ')')
        {
            (*token)->type = OPERATOR_RIGHT_PARENTHESIS;
        }
    }
    /* Reconnaissasnce d'un mot réservé */
    // else if ()
    // {
    //     return (&(*token)->next);
    // }
    return (&(*token)->next);
}

/**
 * @brief Vérifie si le caractère est une quote, un opérateur ou un
 * séparateur de commande.
 * @param c Caractère
 * @param ifs Tableau des séparateurs
 * @return Le type de caractère.
 */
static unsigned char is_special(const uint8_t *command, size_t pos, const uint8_t *ifs)
{
    if (command[pos] == '\\' || command[pos] == '"' || command[pos] == '\'')
    {
        return (IS_A_QUOTE);
    }
    if (command[pos] == '$' || command[pos] == '`')
    {
        return (IS_AN_EXPANSION);
    }
    // if (command[pos] == ')' || command[pos] == '}')
    // {
    //     return (IS_A_DELIMITER);
    // }
    if (command[pos] == '&' || command[pos] == '|' || command[pos] == ';' || command[pos] == '<'
        || command[pos] == '>')
    {
        return (IS_AN_OPERATOR);
    }
    if (command[pos] == '#'
        || (command[pos] == '/' && (command[pos + 1] == '*' || command[pos + 1] == '/')))
    {
        return (IS_A_COMMENT);
    }
    if (ifs != NULL)
    {
        while (*ifs)
        {
            if (command[pos] == *ifs)
            {
                return (IS_A_SEPARATOR);
            }
            ifs++;
        }
    }
    return (0x00);
}

/**
 * @brief Vérifie qu'un caractère puisse être ajouté à un token de type
 * opérateur.
 * @param token Token
 * @param head Position dans la commande
 * @param command Comman`de
 * @return 0 ne peut pas être ajouté, 1 autrement.
 */
static char can_form_an_operator(t_token **token, size_t head, const uint8_t *command)
{
    size_t pos = 0;
    size_t len = 0;

    if (token == NULL || *token == NULL)
    {
        return (0);
    }
    pos = (*token)->tail;
    len = head - pos + 1;
    if (ft_memcmp(&command[pos], "&&", len) == 0)
    {
        (*token)->type = OPERATOR_AND;
        return (1);
    }
    if (ft_memcmp(&command[pos], "||", len) == 0)
    {
        (*token)->type = OPERATOR_OR;
        return (1);
    }
    if (ft_memcmp(&command[pos], "<&", len) == 0)
    {
        (*token)->type = OPERATOR_DUPLICATE_INPUT_REDIRECTION;
        return (1);
    }
    if (ft_memcmp(&command[pos], "<>", len) == 0)
    {
        (*token)->type = OPERATOR_READ_WRITE_REDIRECTION;
        return (1);
    }
    if (ft_memcmp(&command[pos], ">&", len) == 0)
    {
        (*token)->type = OPERATOR_DUPLICATE_OUTPUT_REDIRECTION;
        return (1);
    }
    if (ft_memcmp(&command[pos], ">|", len) == 0)
    {
        (*token)->type = OPERATOR_OUTPUT_REDIRECTION;
        return (1);
    }
    if (ft_memcmp(&command[pos], ">>", len) == 0)
    {
        (*token)->type = OPERATOR_APPEND_OUTPUT_REDIRECTION;
        return (1);
    }
    if (ft_memcmp(&command[pos], "<<-", len) == 0) // Permet de valider "<<" et "<<-"
    {
        (*token)->type = HERE_DOCUMENT;
        return (1);
    }
    return (0);
}

/**
 * Vérifie si le token est un opérateur.
 * @param token Token à vérifier
 * @return 0 le token n'est pas un opérateur, 1 autrement.
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

static int token_is_a_newline(const t_token **token)
{
    return (token != NULL && *token != NULL && (*token)->type == NEWLINE);
}

static size_t can_form_an_expansion(t_token      **token,
                                    size_t         head,
                                    const uint8_t *command,
                                    const uint8_t *ifs)
{
    if (*token == NULL)                         // Si le token courant est NULL...
    {
        *token = ft_token_new(EXPANSION, head); // Création d'un nouveau token !
    }
    else
    {
        (*token)->type = EXPANSION;
    }
    if (command[head] == '$')
    {
        if (command[head + 1] == '{') // ${ : Parameter Expansion
        {
            head += ft_token_recognition(&(*token)->nested_token,
                                         command + head + 2,
                                         (uint8_t *) "}",
                                         ifs)
                + 1;
        }
        else if (command[head + 1] == '(')
        {
            if (command[head + 2] == '(') // $(( : Arithmetic Expansion
            {
                head += ft_token_recognition(&(*token)->nested_token,
                                             command + head + 3,
                                             (uint8_t *) "))",
                                             ifs)
                    + 2;
            }
            else // $( : Command Substitution
            {
                head += ft_token_recognition(&(*token)->nested_token,
                                             command + head + 2,
                                             (uint8_t *) ")",
                                             ifs)
                    + 1;
            }
        }
    }
    else if (command[head] == '`') // Backquotes : Command Substitution
    {
        head += ft_token_recognition(&(*token)->nested_token,
                                     command + head + 1,
                                     (uint8_t *) "`",
                                     ifs);
    }
    // (*token)->head = head;
    return (head);
}

static int is_end_of_input(const uint8_t *start, const uint8_t *end_of_input)
{
    if (end_of_input == NULL || end_of_input[0] == '\0')
    {
        return (0);
    }
    return (ft_memcmp(start, end_of_input, ft_ustrlen(end_of_input)) == 0);
}

size_t ft_token_recognition(t_token **token, const uint8_t *command, const uint8_t *end_of_input, const uint8_t *ifs)
{
    t_token     **current_token = token;
    size_t        iter          = 0;
    unsigned char character[2]  = { 0 };

    /**
     * "character" fonctionne comme ceci:
     * 00000000   00000000   <- début
     * caractère     |||||__ bit indiquant que le caractère courant est une quote
     * de quote      ||||___ bit indiquant que le caractère courant est un opérateur
     * ou d'         |||____ bit indiquant que le caractère courant est un commentaire
     * expansion     ||_____ bit indiquant que le caractère courant est un séparateur de mot
     *               |______ bit indiquant que le caractère courant est un délimiteur d'expansion
     */

    while (command[iter] != '\0')
    {
        CLEAR_BIT(character[0]);
        character[0] |= is_special(command, iter, ifs);

        if (is_end_of_input(command + iter, end_of_input)) // Caractères de fin ...
        {
            if (!CHARACTER_IS_QUOTED(character))           // ET qu'ils ne sont pas quotés ...
            {
                iter += ft_ustrlen(end_of_input);          // Incrément de l'itérateur
                break;                                     // Fin !
            }
        }
        if (token_is_an_operator((const t_token **) current_token))           // Si le token courant est un opérateur ...
        {
            if ((CHARACTER_IS_AN_OPERATOR(character) || command[iter] == '-') // Si le caractère courant est un opérateur ou '-',
                && can_form_an_operator(current_token, iter, command))        // Et qu'ils peuvent former un opérateur ...
            {
                iter++;
                continue;                                                        // Caractère suivant.
            }
            current_token = ft_token_end(current_token, iter, command);          // Sinon fin de token !
        }
        if (CHARACTER_IS_A_QUOTE(character))                                     // Si le caractère courant est une quote ... ( \ ou " ou ' )
        {
            if (!CHARACTER_IS_QUOTED(character))                                 // ET qu'il n'est pas quoté ...
            {
                QUOTE_VALUE(character) = (unsigned char) command[iter];          // Sauvegarde du caractère de quote.
                if (current_token == NULL || *current_token == NULL)             // Si le nouveau token est NULL ...
                {
                    current_token  = ft_token_end(current_token, iter, command); // Fin de token !
                    *current_token = ft_token_new(TOKEN, iter);                  // Création d'un nouveau token !
                }
            }
            else if (QUOTE_VALUE(character) == (unsigned char) command[iter]) // OU qu'il termine la quote existante,
            {
                QUOTE_VALUE(character) = 0;                                   // Reset de la quote sauvegardée.
            }
        }
        else if (CHARACTER_IS_AN_EXPANSION(character)) // Si le caractère courant est le début d'une expansion ... ( $ ou ` )
        {
            if (!CHARACTER_IS_QUOTED(character))       // ET qu'il n'est pas quoté ...
            {
                iter = can_form_an_expansion(current_token, iter, command, ifs);
            }
        }
        else if (CHARACTER_IS_AN_OPERATOR(character))                        // Si le caractère courant est un opérateur ...
        {
            if (!CHARACTER_IS_QUOTED(character))                             // ET qu'il n'est pas déjà quotée ...
            {
                current_token  = ft_token_end(current_token, iter, command); // Fin de token !
                *current_token = ft_token_new(OPERATOR, iter);               // Création d'un nouveau token !
            }
        }
        else if (CHARACTER_IS_NEWLINE(command[iter]))                        // Si le caractère courant est une newline (\n)...
        {
            if (!CHARACTER_IS_QUOTED(character)                              // Qu'il n'est pas quoté...
                && !token_is_a_newline((const t_token **) current_token))    // Et que le token n'est pas de type NEWLINE ...
            {
                current_token  = ft_token_end(current_token, iter, command); // Fin de token !
                *current_token = ft_token_new(NEWLINE, iter);                // Création d'un nouveau token !
                current_token  = ft_token_end(current_token, iter, command); // Fin de token !
            }
        }
        else if (CHARACTER_IS_A_SEPARATOR(character))                       // Si le caractère courant est un séparateur... (généralement un whitespace)
        {
            if (!CHARACTER_IS_QUOTED(character))                            // ET qu'il n'est pas quoté...
            {
                current_token = ft_token_end(current_token, iter, command); // Fin de token !
            }
        }
        else if (CHARACTER_IS_A_COMMENT(character))                         // Si le caractère courant est le début d'un commentaire (#)...
        {
            if (!CHARACTER_IS_QUOTED(character))                            // ET qu'il n'est pas quoté...
            {
                current_token = ft_token_end(current_token, iter, command); // Fin du token précédent !
                if (command[iter] == '#' || ft_memcmp(command + iter, "//", 2) == 0)
                {
                    while (command[iter] != '\0'
                           && command[iter] != '\n') // Déplacement jusqu'à la fin de la ligne.
                    {
                        iter++;
                    }
                    continue;
                }
                if (ft_memcmp(command + iter, "/*", 2) == 0)
                {
                    iter += 2;
                    while (command[iter] != '\0'
                           && ft_memcmp(command + iter, "*/", 2) != 0) // Déplacement jusqu'à la fin des caractères fermants du commentaire.
                    {
                        iter++;
                    }
                    if (command[iter] != '\0')
                    {
                        iter += 2;
                    }
                    continue;
                }
            }
        }
        if (*current_token == NULL && !CHARACTER_IS_A_SEPARATOR(character)) // Si le token courant est NULL et pas un séparateur de mot ...
        {
            *current_token = ft_token_new(TOKEN, iter);                     // Création d'un nouveau token !
        }
        if (QUOTE_VALUE(character) == '\\' && iter > 0                      // Si le caractère de quote est '\' et que le
            && command[iter - 1] == '\\')                                   // caractère précédent était un '\'...
        {
            QUOTE_VALUE(character) = 0;                                     // Reset de la quote sauvegardée.
        }
        iter++;
    }

    (void) ft_token_end(current_token, iter, command); // Fin de token !

    return (iter);
}

void ft_free_token_list(t_token *token)
{
    t_token *next = NULL;

    while (token != NULL)
    {
        next = token->next;
        ft_free_token_list(token->nested_token);
        free(token);
        token = next;
    }
}

const char *ft_token_type_to_str(e_token_type type)
{
    switch (type)
    {
        case TOKEN:
            return "TOKEN";
        case OPERATOR:
            return "OPERATOR";
        case NEWLINE:
            return "NEWLINE";
        case EXPANSION:
            return "EXPANSION";
        case WORD:
            return "WORD";
        case OPERATOR_AMPERSAND:
            return "OPERATOR_AMPERSAND";
        case OPERATOR_PIPE:
            return "OPERATOR_PIPE";
        case OPERATOR_SEMICOLON:
            return "OPERATOR_SEMICOLON";
        case OPERATOR_INPUT_REDIRECTION:
            return "OPERATOR_INPUT_REDIRECTION";
        case OPERATOR_OUTPUT_REDIRECTION:
            return "OPERATOR_OUTPUT_REDIRECTION";
        case OPERATOR_LEFT_PARENTHESIS:
            return "OPERATOR_LEFT_PARENTHESIS";
        case OPERATOR_RIGHT_PARENTHESIS:
            return "OPERATOR_RIGHT_PARENTHESIS";
        case OPERATOR_AND:
            return "OPERATOR_AND";
        case OPERATOR_OR:
            return "OPERATOR_OR";
        case OPERATOR_DUPLICATE_INPUT_REDIRECTION:
            return "OPERATOR_DUPLICATE_INPUT_REDIRECTION";
        case OPERATOR_DUPLICATE_OUTPUT_REDIRECTION:
            return "OPERATOR_DUPLICATE_OUTPUT_REDIRECTION";
        case OPERATOR_READ_WRITE_REDIRECTION:
            return "OPERATOR_READ_WRITE_REDIRECTION";
        case OPERATOR_APPEND_OUTPUT_REDIRECTION:
            return "OPERATOR_APPEND_OUTPUT_REDIRECTION";
        case HERE_DOCUMENT:
            return "HERE_DOCUMENT";
        case IO_NUMBER:
            return "IO_NUMBER";
    }
}
