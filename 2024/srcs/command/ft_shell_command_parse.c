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

#include "ft_shell_command.h"
#include "ft_shell_token.h"
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
                ft_memcpy(buffer + pos, buffer + pos + 2, command->len - pos); // bizarre attention
            }
            else
            {
                pos++;
            }
        }
    }
    return (buffer);
}

void ft_shell_command_parse(const t_cmd *command, const char *ifs)
{
    uint8_t *buffer     = NULL;
    t_token *token_list = NULL;
    // char   **args       = NULL;
    // char    *cmd        = NULL;
    // t_args  *list       = NULL;

    buffer = ft_prepare_command(command);
    (void) ft_shell_token_recognition(&token_list,
                                      buffer,
                                      (const uint8_t *) "\n",
                                      (const uint8_t *) ifs);
    ft_shell_token_debug(token_list);
    // if ((cmd = ft_lexical_analysis(shell->command->buffer))
    // && (args = ft_split_whitespaces(cmd))
    // && !ft_syntax_analysis(args))
    // {
    //     if ((list = ft_analyse(args, 0, shell)))
    //     {
    //         ft_run_parser(list, shell);
    //     }
    // }
    free(buffer);
    ft_shell_token_delete(token_list);
    // ft_freetab(&args);
    // free(cmd);
    // ft_free_list(&list);
}
