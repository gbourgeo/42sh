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
#include "ft_shell_constants.h"
#include "ft_shell_log.h"
#include "ft_shell_terminal.h"
#include "libft.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static uint8_t *ft_prepare_command(const uint8_t *commandbuf, size_t buflen)
{
    uint8_t *buffer = ft_ustrdup(commandbuf);
    size_t   pos    = 0;

    if (buffer == NULL)
    {
        return (NULL);
    }
    while (pos < buflen)
    {
        if (buffer[pos] == '\\' && buffer[pos + 1] == '\n')
        {
            size_t cpylen = buflen - pos - 2;

            ft_memcpy(buffer + pos, buffer + pos + 2, cpylen);
            buflen -= 2;
            continue;
        }
        pos++;
    }
    buffer[pos] = '\0';
    return (buffer);
}

e_comparse ft_shell_command_parse(t_cmd      *command,
                                  const char *ifs,
                                  uint32_t    options,
                                  t_term     *terminal)
{
    /* Si la commande vient de l'historique et qu'elle a été modifiée,
    * on refait la reconnaissance de token */
    if (_test_bit(command->option, COMMAND_HISTORIC_MODIFIED))
    {
        ft_command_token_remove_all(command->token);
        command->token = NULL;
        free(command->tokenbuf);
        command->tokenbuf = NULL;
    }
    if (command->token == NULL)
    {
        command->tokenbuf = ft_prepare_command(command->buffer, command->len);

        if (ifs != NULL && *ifs == '\0')
        {
            ifs = "  \t\n";
        }
        command->pos = ft_command_token_recognition(&command->token,
                                                    (const uint8_t *) command->tokenbuf,
                                                    (const uint8_t *) "",
                                                    (const uint8_t * const) ifs);
    }
    if (_test_bit(options, SHELL_DEBUG_MODE))
    {
        ft_shell_log(SH_LOG_LEVEL_DBG, "Line: %s", command->buffer);
        ft_command_token_debug(command->token, command->tokenbuf, terminal);
    }
    return (SHELL_COMMAND_PARSED);
}
