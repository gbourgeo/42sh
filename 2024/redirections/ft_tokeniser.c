/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokeniser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/03/05 10:59:50 by gbourgeo          #+#    #+#             */
/*   Updated: 2014/03/30 00:31:44 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

#include "ft_builtins.h"
#include "ft_token.h"

t_token     *ft_new_token(void)
{
    t_token *token;

    token = (t_token*)malloc(sizeof(*token));
    if (token != NULL)
    {
        token->type = TOKEN;
        token->ptr = NULL;
        token->len = 0;
        token->next = NULL;
    }
    return (token);
}

void        ft_free_token_list(t_token *token_list)
{
    if (token_list == NULL)
        return;
    if (token_list->next)
        ft_free_token_list(token_list->next);
    free(token_list);
}

static bool is_field_separator(char c, char * const * env)
{
    const char  *ifs;

    ifs = ft_getenv("IFS", env);
    if (ifs != NULL)
    {
        while (*ifs)
        {
            if (*ifs == c)
            {
                return (true);
            }
            ifs++;
        }
    }
    return (false);
}

t_token     *ft_tokenise_command(const char *command, char * const * env)
{
    t_token *token_list;
    t_token **token;
    int     i;

    token_list = NULL;
    token = NULL;
    i = 0;
    while (command[i])
    {
        if (is_field_separator(command[i], env))
        {
            if (token != NULL)
            {
                if (token_list == NULL)
                    token_list = *token;
                token = &(*token)->next;
                *token = ft_new_token();
            }
        }
        i++;
    }
    return (token_list);
}
