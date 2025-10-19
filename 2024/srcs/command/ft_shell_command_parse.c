/****************************************************************************/
/*                                                                          */
/*                                                       :::      ::::::::  */
/*  ft_shell_command_parse.c                           :+:      :+:    :+:  */
/*                                                   +:+ +:+         +:+    */
/*  By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+       */
/*                                               +#+#+#+#+#+   +#+          */
/*  Created: 2014/03/05 10:59:50 by gbourgeo          #+#    #+#            */
/*  Updated: 2014/03/30 00:31:44 by gbourgeo         ###   ########.fr      */
/*                                                                          */
/****************************************************************************/

#include "ft_defines.h"
#include "ft_shell_command.h"
#include "libft.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static uint8_t *ft_prepare_command(const t_cmd *command)
{
    uint8_t *buffer = ft_ustrdup(command->buffer);
    size_t   pos    = 0;

    if (buffer != NULL)
    {
        while (buffer[pos] != '\0')
        {
            if (buffer[pos] == '\\' && buffer[pos + 1] == '\n')
            {
                size_t len = command->len - pos - 2;

                ft_memcpy(buffer + pos, buffer + pos + 2, len); // bizarre attention
                buffer[pos + len] = '\0';
            }
            else
            {
                pos++;
            }
        }
        buffer[pos] = '\0';
    }
    return (buffer);
}

e_comparse ft_shell_command_parse(t_cmd *command, const char *ifs)
{
    /* Si la commande vient de l'historique et qu'elle a été modifiée,
     * on refait la reconnaissance de token */
    if (TEST_BIT(command->option, COMMAND_HISTORIC_MODIFIED))
    {
        ft_command_token_remove_all(command->token);
        command->token = NULL;
    }
    if (command->token == NULL)
    {
        uint8_t *buffer = NULL;

        buffer = ft_prepare_command(command);
        if (ifs != NULL && *ifs == '\0')
        {
            ifs = "  \t\n";
        }
        command->pos = ft_command_token_recognition(&command->token,
                                                    (const uint8_t *) buffer,
                                                    (const uint8_t *) "",
                                                    (const uint8_t * const) ifs);
        ft_command_token_debug(command->token, buffer);
        free(buffer);
    }
    return (SHELL_COMMAND_PARSED);
}
