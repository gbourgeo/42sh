/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_command_token_debug.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/03/05 10:59:50 by gbourgeo          #+#    #+#             */
/*   Updated: 2014/03/30 00:31:44 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "ft_shell_command.h"
#include "ft_shell_log.h"
#include "ft_shell_terminal.h"
#include "ft_snprintf.h"
#include <stdint.h>
#include <stdlib.h>

/************************************/
/* FONCTIONS PRIVEES                */
/************************************/
static const t_token *internal_token_print(const t_token *first,
                                           const t_token *last,
                                           const int     *printedlentable,
                                           size_t         pos,
                                           const uint8_t *buffer);
static const char    *internal_token_type_to_str(e_token_type type);
static size_t         internal_token_nb(const t_token *token);

/************************************/
/* FONCTIONS PUBLIQUES              */
/************************************/
void ft_command_token_debug(const t_token *token,
                            const uint8_t *buffer,
                            const t_term  *terminal)
{
    size_t               nb_tokens       = 0;     /* Nombre de Token */
    size_t               token_pos       = 0;     /* Position du Token */
    const t_token       *first_token     = NULL;  /* Premier Token d'une ligne d'écriture */
    size_t               first_pos       = 0;     /* Position du premier token */
    int                 *printedlentable = NULL;  /* Table de sauvegarde de la plus grande taille entre la description du Token et sa valeur */
    int                  maxlen          = 0;     /* Plus grande taille entre la description du Token et sa valeur */
    int                  printed         = 0;     /* Longueur de caractères déjà imprimés */
    char                 token_desc[256] = { 0 }; /* Description Token : son type et de sa longueur */
    int                  desc_len        = 0;     /* Longueur de la description */

    // Calcul du nombre total de token
    nb_tokens = internal_token_nb(token);
    // Aucun token ? on quitte.
    if (nb_tokens == 0)
    {
        return;
    }
    /* Allocation de la table des grandeurs max */
    printedlentable = malloc(nb_tokens * sizeof(*printedlentable));
    ft_term_move_cursor(terminal, MOVE_CURSOR_END);
    ft_term_move_cursor_down(terminal);
    ft_term_clear_line_and_under(terminal);
    ft_shell_log(SH_LOG_LEVEL_DBG, "Token nb  :  %d", nb_tokens);
    ft_shell_log(SH_LOG_LEVEL_DBG, "Token list:  TYPE (length) | ...");
    first_token = token;
    while (token != NULL)
    {
        /* Ecriture de la description du Token et
         * récupération de la longueur de la description. */
        desc_len = ft_snprintf(token_desc,
                               sizeof(token_desc),
                               "%s%s (%d)",
                               (token == first_token) ? "  " : " | ",
                               internal_token_type_to_str(token->type),
                               token->head - token->tail);
        /* Récupération de la longueur max */
        if (desc_len > (int) (token->head - token->tail))
        {
            maxlen = desc_len;
        }
        else
        {
            maxlen = (int) (token->head - token->tail) + ((token == first_token) ? 2 : 3);
        }
        /* Sauvegarde de la longueur max */
        printedlentable[token_pos] = (token->type == NEWLINE) ? 1 : maxlen;
        /* S'il y a assez d'espace pour imprimer la description */
        if (printed + maxlen < terminal->max_column)
        {
            printed += ft_printf("%*s", -maxlen, token_desc);
            if (token->type == NEWLINE)
            {
                first_token = internal_token_print(first_token, token->next, printedlentable, first_pos, buffer);
                first_pos   = token_pos + 1;
                printed     = 0;
            }
            token = token->next;
            token_pos++;
        }
        /* S'il n'y a pas assez d'espace pour imprimer la description */
        else
        {
            /* On tronque la description */
            if (token == first_token)
            {
                ft_printf("%*s", -terminal->max_column, token_desc);
            }
            first_token = internal_token_print(first_token, token, printedlentable, first_pos, buffer);
            first_pos   = token_pos;
            printed     = 0;
        }
    }
    if (first_token != NULL)
    {
        internal_token_print(first_token, token, printedlentable, first_pos, buffer);
    }
    free(printedlentable);
}

static size_t internal_token_nb(const t_token *token)
{
    size_t nbr = 0;

    while (token)
    {
        nbr++;
        token = token->next;
    }
    return (nbr);
}

static const t_token *internal_token_print(const t_token *first,
                                  const t_token *last,
                                  const int     *printedlentable,
                                  size_t         pos,
                                  const uint8_t *buffer)
{
    const t_token *token            = first;
    char           token_value[256] = { 0 };
    int            desc_len         = 0;

    if (first->type != NEWLINE)
    {
        ft_printf("\n");
    }
    while (token != last)
    {
        desc_len = ft_snprintf(token_value,
                               sizeof(token_value),
                               "%s%.*s",
                               (token == first) ? "  " : "   ",
                               token->head - token->tail,
                               buffer + token->tail);
        ft_printf("\033[36m%*s\033[0m", -printedlentable[pos], token_value);
        pos++;
        token = token->next;
        (void) desc_len;
    }
    ft_printf("\n");
    return (last);
}

static const char *internal_token_type_to_str(e_token_type type)
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
