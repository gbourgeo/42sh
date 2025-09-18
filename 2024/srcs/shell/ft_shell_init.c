/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_shell_init.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/02/09 03:00:47 by gbourgeo          #+#    #+#             */
/*   Updated: 2014/03/27 18:39:53 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_defines.h"
#include "ft_shell.h"
#include "ft_shell_builtins.h"
#include "ft_shell_history.h"
#include "ft_shell_prompt.h"
#include "ft_shell_terminal.h"
#include "ft_snprintf.h"
#include "libft.h"
#include <fcntl.h>
#include <pwd.h>
#include <signal.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/param.h>
#include <sys/ttycom.h>
#include <unistd.h>

/**
 * @brief Fonction d'initialisation de l'environnement de l'utilisateur, à savoir:
 *
 * - Duplication des variables d'environnement du processus appelant (parent),
 *
 * - Ajout/Modification de la variable d'environnement PWD,
 *
 * - Ajout/Modification de la variable d'environnement SHLVL,
 *
 * - Ajout/Modification de la variable d'environnement SHELL.
 * @param[in] shell_name    Nom du shell
 * @param[in] environ       Environnement à dupliquer
 * @return Nouvel environnement.
 */
static char **ft_init_global_environ(const char *shell_name, const char **environ)
{
    char **env = NULL;
    char   ptr[MAXPATHLEN];

    env = ft_tabdup(environ);
    (void) getcwd(ptr, sizeof(ptr));
    env = ft_modify_env("PWD", ptr, env, 0);
    ft_snprintf(ptr, sizeof(ptr), "%d", ft_atoi(ft_real_getenv("SHLVL", (const char **) env)) + 1);
    env = ft_modify_env("SHLVL", ptr, env, 0);
    env = ft_modify_env("SHELL", shell_name, env, 0);
    return (env);
}

/**
 * @brief Fonction d'initialisation de l'environnement du shell, à savoir:
 *
 * - Récupération des lignes et colonnes du terminal pour
 *   ajouter/modifier les variables d'environnement LINES et COLUMNS,
 *
 * - Récupération du login de l'utilisateur courant pour
 *   ajouter/modifier les variables d'environnement HOME et LOGNAME,
 *
 * - Ajout/Modification des variables d'environnement PWD, SHELL, TMPDIR et IFS.
 * @param[in] progname    Nom du programme
 * @return Nouvel environnement.
 */
static char **ft_init_internal_environ(const char *progname)
{
    struct winsize winsize;
    struct passwd *passwd             = NULL;
    char         **environ            = NULL;
    char           buffer[MAXPATHLEN] = { 0 };

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &winsize) != -1)
    {
        ft_snprintf(buffer, sizeof(buffer), "%d", winsize.ws_col);
        environ = ft_modify_env("COLUMNS", buffer, environ, 0);
        ft_snprintf(buffer, sizeof(buffer), "%d", winsize.ws_row);
        environ = ft_modify_env("LINES", buffer, environ, 0);
    }
    passwd = getpwuid(getuid());
    if (passwd != NULL)
    {
        environ = ft_modify_env("HOME", passwd->pw_dir, environ, 0);
        environ = ft_modify_env("LOGNAME", passwd->pw_name, environ, 0);
    }
    (void) getcwd(buffer, sizeof(buffer));
    environ = ft_modify_env("PWD", buffer, environ, 0);
    environ = ft_modify_env("SHELL", progname, environ, 0);
    environ = ft_modify_env("TMPDIR", "/tmp", environ, 0);
    environ = ft_modify_env("IFS", " \n\t", environ, 1);
    environ = ft_modify_env("42SH_HISTORY_SIZE", SHELL_HISTORY_MAX_ELEMS, environ, 0);
    environ = ft_modify_env("PS1", "%F{white}%p %F{yellow}%u %F{red}%w %F{white} > ", environ, 0);
    return (environ);
}

void ft_shell_init(const char *progname, const char **environ, t_shell *shell)
{
    ft_shell_terminal_init(&shell->terminal);
    ft_shell_prompt_init(&shell->prompt);
    ft_shell_history_init(&shell->history);
    shell->command      = NULL;
    shell->yank         = NULL;
    shell->progname     = ft_strrchr(progname, '/');
    shell->progname     = (shell->progname != NULL) ? shell->progname + 1 : progname;
    shell->bin_path     = ft_strdup("/usr/gnu/bin:/usr/local/bin:/usr/bin:/bin");
    shell->global_env   = ft_init_global_environ(shell->progname, environ);
    shell->internal_env = ft_init_internal_environ(shell->progname);
    for (size_t iter = 0; iter < LENGTH_OF(shell->sigs); iter++)
    {
        shell->sigs[iter] = SIG_ERR;
    }
    shell->options = 0;
    shell->quit    = 0;
    shell->status  = 0;
}
