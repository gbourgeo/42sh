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

#include "ft_shell.h"
#include "ft_shell_builtins.h"
#include "ft_shell_command.h"
#include "ft_shell_token.h"
#include "libft.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

// static void     ft_exec_pipes(t_args *list, t_shell *e)
// {
//     if ((list->type == DOUBLE_AND && e->status == 0)
//         || (list->type == DOUBLE_OR && e->status != 0)
//         || (list->type == 0))
//         ft_pipex_or_exec(list, e);
// }

// static void     ft_exec(t_args *list, t_shell *e)
// {
//     if ((list->type == DOUBLE_AND && e->status == 0)
//         || (list->type == DOUBLE_OR && e->status != 0)
//         || (list->type == 0))
//         e->status = check_and_exec((const char **)list->args, e);
// }

// static void     ft_run_parser(t_args *list, t_shell *e)
// {
//     pid_t       pid;

//     ft_shell_terminal_restore_attributes(e);
//     while (list != NULL)
//     {
//         if (list->args && *list->args && list->pipe)
//         {
//             pid = fork();
//             if (pid == 0)
//             {
//                 ft_exec_pipes(list, e);
//                 exit(e->status);
//             }
//             else if (pid > 0)
//                 waitpid(pid, &e->status, 0);
//             else
//                 ft_log(SH_LOG_LEVEL_WARN, "Fork failed");
//         }
//         else if (list->args && *list->args)
//             ft_exec(list, e);
//         list = list->next;
//     }
//     ft_shell_terminal_change_attributes(e);
// }

// static t_args   *ft_analyse(char **args, int type, t_shell *e)
// {
//     int         i;
//     t_args      *list;

//     i = 0;
//     if (!args || !*args || (list = ft_argsnew(args, type)) == NULL)
//         return (NULL);
//     while (args[i] && (ft_strcmp(args[i], ";") && ft_strcmp(args[i], "||")
//             && ft_strcmp(args[i], "&&")))
//     {
//         if (ft_strcharset(args[i], "<>|") && ft_strcmp(args[i], "||"))
//             list->pipe++;
//         i++;
//     }
//     if (!ft_strcmp(args[i], "||"))
//         list->next = ft_analyse(&args[i + 1], DOUBLE_OR, e);
//     else if (!ft_strcmp(args[i], "&&"))
//         list->next = ft_analyse(&args[i + 1], DOUBLE_AND, e);
//     else if (!ft_strcmp(args[i], ";"))
//         list->next = ft_analyse(&args[i + 1], 0, e);
//     free(list->args[i]);
//     list->args[i] = NULL;
//     return (list);
// }

static uint8_t *ft_prepare_command(t_cmd *command)
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
                command->len -= 2;
            }
            else
            {
                pos++;
            }
        }
    }
    return (buffer);
}

/**
 * @brief Parsing de commande:
 *
 * - Tokenisation
 */
void ft_shell_command_parse(t_shell *shell)
{
    uint8_t *buffer     = NULL;
    t_token *token_list = NULL;
    char   **args       = NULL;
    char    *cmd        = NULL;
    t_args  *list       = NULL;

    buffer = ft_prepare_command(shell->command);
    (void) ft_token_recognition(&token_list,
                                buffer,
                                (const uint8_t *) "",
                                (const uint8_t *) ft_getenv("IFS", shell));
    debug_tokens(token_list, shell);
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
    ft_free_token_list(token_list);
    ft_freetab(&args);
    free(cmd);
    ft_free_list(&list);
}
