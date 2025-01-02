/****************************************************************************/
/*                                                                          */
/*                                                       :::      ::::::::  */
/*  ft_parser.c                                        :+:      :+:    :+:  */
/*                                                   +:+ +:+         +:+    */
/*  By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+       */
/*                                               +#+#+#+#+#+   +#+          */
/*  Created: 2014/03/05 10:59:50 by gbourgeo          #+#    #+#            */
/*  Updated: 2014/03/30 00:31:44 by gbourgeo         ###   ########.fr      */
/*                                                                          */
/****************************************************************************/

#include "ft_builtins.h"
#include "ft_shell.h"
#include "ft_token.h"
#include "libft.h"
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

//     ft_restore_terminal_attributes(e);
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
//     ft_change_terminal_attributes(e);
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

/**
 * @brief Parsing de commande:
 *
 * - Tokenisation
 */
void ft_parse_command(t_shell *shell)
{
    t_token *token_list = NULL;
    char   **args       = NULL;
    char    *cmd        = NULL;
    t_args  *list       = NULL;

    ft_token_recognition(&token_list, shell->command.buffer, ft_getenv("IFS", shell));
    debug_tokens(token_list, shell);
    // if ((cmd = ft_lexical_analysis(shell->command.buffer))
    // && (args = ft_split_whitespaces(cmd))
    // && !ft_syntax_analysis(args))
    // {
    //     if ((list = ft_analyse(args, 0, shell)))
    //     {
    //         ft_run_parser(list, shell);
    //     }
    // }
    ft_free_list(&list);
    ft_free_token_list(token_list);
    ft_freetab(&args);
    free(cmd);
}
