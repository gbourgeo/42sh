/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_shell_token_debug.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/03/05 10:59:50 by gbourgeo          #+#    #+#             */
/*   Updated: 2014/03/30 00:31:44 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_defines.h"
#include "ft_printf.h"
#include "ft_shell.h"
#include "ft_shell_constants.h"
#include "ft_shell_log.h"
#include "ft_shell_token.h"
#include "ft_snprintf.h"
#include "libft.h"
#include <stdlib.h>

static const t_token *print_token(const t_token *first,
                                  const t_token *last,
                                  const int     *saved_len,
                                  const t_shell *shell)
{
    char info[256] = { 0 };
    long info_len  = 0;
    int  nb_tokens = 0;

    ft_printf("\n");
    while (first != last)
    {
        info_len = ft_snprintf(info,
                               sizeof(info),
                               "%.*s",
                               first->head - first->tail,
                               shell->command->buffer + first->tail);
        for (long i = info_len; i < saved_len[nb_tokens]; i++)
        {
            ft_strncat(info, " ", sizeof(info));
        }
        ft_printf("\033[36m%s%s\033[0m", (nb_tokens == 0) ? "  " : "   ", info);
        nb_tokens++;
        first = first->next;
    }
    ft_printf("\n");
    return (first);
}

void ft_shell_token_debug(const t_token *token)
{
    extern const t_shell g_shell;
    char                 info[256]  = { 0 };
    int                  info_len   = 0;
    const t_token       *ptr        = NULL;
    int                  start_save = 0;
    int                  nb_tokens  = 0;
    int                  printed    = 0;
    int                 *saved_len  = NULL;

    // Debug mode off ? on quitte.
    if (!TEST_BIT(g_shell.options, SHELL_DEBUG_MODE))
    {
        return;
    }
    ft_shell_log(SH_LOG_LEVEL_DBG, "Line: %s", g_shell.command->buffer);
    // Calcul du nombre total de token
    ptr = token;
    while (ptr)
    {
        nb_tokens++;
        ptr = ptr->next;
    }
    // Aucun token ? on quitte.
    if (nb_tokens == 0)
    {
        return;
    }
    saved_len = malloc((size_t) nb_tokens * sizeof(*saved_len));
    ptr       = token;
    ft_shell_log(SH_LOG_LEVEL_DBG, "Token list:  TYPE (length) | ...");
    nb_tokens = 0;
    while (token)
    {
        info_len = ft_snprintf(info,
                               sizeof(info),
                               "%s (%d)",
                               ft_token_type_to_str(token->type),
                               token->head - token->tail);
        if (printed + info_len + 3 < g_shell.terminal.max_column)
        {
            saved_len[nb_tokens] = (info_len > (int) (token->head - token->tail)) ?
                info_len :
                (int) (token->head - token->tail);
            for (long i = info_len; i < saved_len[nb_tokens]; i++)
            {
                ft_strncat(info, " ", sizeof(info));
            }
            printed += ft_printf("%s%s", (token == ptr) ? "  " : " | ", info);
            nb_tokens++;
            token = token->next;
        }
        else                  // On est arrivé à écrire jusqu'à la fin du terminal
        {
            if (ptr == token) // On a rien écrit, terminal trop petit, on quitte
            {
                ft_printf(" \033[31mAborted\033[0m\n");
                break;
            }
            ptr        = print_token(ptr, token, saved_len + start_save, &g_shell);
            start_save = nb_tokens;
            printed    = 0;
        }
    }
    print_token(ptr, token, saved_len + start_save, &g_shell);
    free(saved_len);
}
