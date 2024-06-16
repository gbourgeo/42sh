/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init_shell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/02/09 03:00:47 by gbourgeo          #+#    #+#             */
/*   Updated: 2014/03/27 18:39:53 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

#include "ft_snprintf.h"
#include "ft_minishell.h"

static void ft_tty_check(t_shell *shell)
{
    char    *tty_name;

    tty_name = ttyname(STDIN_FILENO);
    if (isatty(STDIN_FILENO) == 0)
        ft_error_fatal(shell, "Terminal not valid.");
    if (tty_name == NULL)
        ft_error_fatal(shell, "You are not connected to a terminal.");
    if ((shell->fd = open(tty_name, O_RDWR)) == -1)
        ft_error_fatal(shell, "Failed to open %s", tty_name);
}

static void ft_load_new_capacities(char *term_base, char * const *environ, t_shell *shell)
{
    const char  *term_name;
    int         ret;

    if ((term_name = ft_getenv("TERM", environ)) == NULL)
        term_name = term_base;
    ret = tgetent(NULL, term_name);
    if (ret == -1)
    {
        ft_error(shell, "WARNING: Termcap's data base files unavailable,");
        ft_error(shell, "WARNING: 42sh will certainly not do things right,");
        ft_error(shell, "WARNING: U have been warned.");
    }
    else if (ret == 0)
    {
        ft_error(shell, "WARNING: Terminal not defined in database.");
    }
}

static void ft_update_env(t_shell *shell)
{
    char    *tmp;

    if (ft_getenv("SHLVL", shell->env))
    {
        ft_snprintf(shell->command.buffer,
            shell->command.buffer_size,
            "setenv SHLVL %d",
            ft_atoi(ft_getenv("SHLVL", shell->env)) + 1);
        ft_parse_command(shell);
    }
    if (ft_getenv("SHELL", shell->env))
    {
        tmp = getcwd(NULL, 0);
        ft_snprintf(shell->command.buffer,
            shell->command.buffer_size,
            "setenv SHELL %s/%s",
            tmp,
            shell->progname);
        free(tmp);
        ft_parse_command(shell);
    }
    ft_strclr(shell->command.buffer);
}

static void ft_setup_env(t_shell *shell)
{
    char    *tmp;

    tmp = getcwd(NULL, 0);
    ft_snprintf(shell->command.buffer,
        shell->command.buffer_size,
        "setenv PWD %s",
        tmp);
    free(tmp);
    ft_parse_command(shell);
    ft_strncpy(shell->command.buffer, "setenv SHLVL 1", shell->command.buffer_size - 1);
    ft_parse_command(shell);
    ft_snprintf(shell->command.buffer,
        shell->command.buffer_size,
        "setenv _ %s",
        shell->progname);
    ft_parse_command(shell);
    ft_strclr(shell->command.buffer);
}

void        ft_init_shell(const char **av, char * const *environ, t_shell *shell)
{
    shell->options = 0;
    shell->fd = STDIN_FILENO;
    shell->quit = 0;
    shell->status = 0;
    shell->progname = ft_strrchr(av[0], '/');
    shell->progname = (shell->progname != NULL) ? shell->progname + 1 : av[0];
    shell->bin_path = ft_strdup("/usr/gnu/bin:/usr/local/bin:/usr/bin:/bin");
    shell->env = ft_tabdup(environ);
    shell->prompt.str = NULL;
    shell->prompt.len = 0;
    shell->prompt.print = 0;
    ft_command_init(&shell->command);
    shell->history = ft_history_init(NULL); // TODO: parser le fichier d'historique de commandes
    shell->highlighted.on = 0;
    shell->highlighted.texts = NULL;
    if (tcgetattr(shell->fd, &shell->terminal.ios) != 0)
        return (ft_error_fatal(shell, "Can't get terminal attributes"));
    shell->options |= SHELL_TERMCAPS_LOADED;
    shell->terminal.max_column = 0;
    shell->terminal.max_line = 0;
    shell->terminal.current_column = 0;
    shell->terminal.current_line = 0;
    shell->terminal.start_column = 0;
    shell->terminal.start_line = 0;
    memset(shell->sigs, 0, sizeof(shell->sigs));
    if (shell->bin_path == NULL || shell->env == NULL || shell->command.buffer == NULL)
        ft_error_fatal(shell, "malloc: Can't allocate memory");
}

const char  **ft_get_opt(const char **av, char * const *environ, t_shell *shell)
{
    while (*av && **av == '-')
    {
        if (ft_strcmp(*av, "--") == 0)
            break ;
        if (ft_strcmp(*av, "-d") == 0
        ||  ft_strcmp(*av, "--debug") == 0)
        {
            shell->options |= SHELL_DEBUG_MODE;
        }
        else
        {
            ft_error_fatal(shell, "Invalid option: %s", *av);
            break ;
        }
        av++;
    }
    if (*av == NULL)
    {
        ft_get_terminal_size(shell);
        ft_load_new_capacities("xterm", environ, shell);
        ft_load_termcaps(shell);
        ft_tty_check(shell);
        ft_signals(shell);
        if (ft_tablen(shell->env) == 0)
            ft_setup_env(shell);
        else
            ft_update_env(shell);
        shell->prompt.print = 1;
    }
    return (av);
}