/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2013/12/28 04:47:21 by gbourgeo          #+#    #+#             */
/*   Updated: 2018/04/06 17:43:04 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "get_next_line.h"
#include "ft_termkeys.h"
#include "ft_minishell.h"

t_shell         g_shell;  /* Structure globale de notre shell */

static void     ft_minishell(t_shell *shell)
{
    char        buf[MAX_KEY_SIZE];
    int         ret;

    while (shell->quit == 0)
    {
        if (shell->prompt.print != 0)
        {
            ft_prompt(shell);
            debug(buf, shell);
        }
        ft_bzero(buf, sizeof(buf));
        ret = read(shell->fd, buf, sizeof(buf));
        if (ret <= 0)
        {
            ft_error(shell, "read error (%d)", ret);
            shell->status = 127;
            break;
        }
        ft_key_read(buf, ret, shell);
        if (shell->prompt.print == 0)
            debug(buf, shell);
    }
}

static void     ft_minishell_file(const char **av, t_shell *shell)
{
    char        *line;

    line = NULL;
    shell->fd = open(av[1], O_RDONLY);
    if (shell->fd == -1)
    {
        ft_error(shell, "Unable to open file: %s", av[1]);
        return ;
    }
    if (ft_dup2(shell->fd, 0) >= 0)
    {
        while (get_next_line(shell->fd, &line) > 0)
        {
            if (ft_strstr("exit", line))
                break ;
            ft_parse_command(shell);
            ft_freestr(&line);
            if (!shell->status)
                break ;
        }
    }
    close(shell->fd);
}

int             main(int ac, const char **av)
{
    extern char * const *environ;

    (void)ac;
    ft_init_shell(av, environ, &g_shell);
    if (g_shell.quit == 0)
    {
        av = ft_get_opt(av + 1, environ, &g_shell);
        if (g_shell.quit == 0)
        {
            if (*av)
                ft_minishell_file(av, &g_shell);
            else
                ft_minishell(&g_shell);
        }
    }
    ft_free_shell(&g_shell);
    return (g_shell.status);
}
