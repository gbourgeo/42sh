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

#include "ft_builtins.h"
#include "ft_command.h"
#include "ft_highlight.h"
#include "ft_history.h"
#include "ft_prompt.h"
#include "ft_shell.h"
#include "ft_snprintf.h"
#include "ft_termios.h"
#include "libft.h"
#include <fcntl.h>
#include <pwd.h>
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
static char **ft_init_global_environ(const char *shell_name, char * const *environ)
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
    char          *login              = NULL;
    char           buffer[MAXPATHLEN] = { 0 };

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &winsize) != -1)
    {
        ft_snprintf(buffer, sizeof(buffer), "%d", winsize.ws_col);
        environ = ft_modify_env("COLUMNS", buffer, environ, 0);
        ft_snprintf(buffer, sizeof(buffer), "%d", winsize.ws_row);
        environ = ft_modify_env("LINES", buffer, environ, 0);
    }
    login = getlogin();
    if (login != NULL)
    {
        passwd = getpwnam(login);
        if (passwd != NULL)
        {
            environ = ft_modify_env("HOME", passwd->pw_dir, environ, 0);
            environ = ft_modify_env("LOGNAME", passwd->pw_name, environ, 0);
        }
    }
    (void) getcwd(buffer, sizeof(buffer));
    environ = ft_modify_env("PWD", buffer, environ, 0);
    environ = ft_modify_env("SHELL", progname, environ, 0);
    environ = ft_modify_env("TMPDIR", "/tmp", environ, 0);
    environ = ft_modify_env("IFS", " \n\t", environ, 1);
    return (environ);
}

void ft_shell_init(const char *progname, char * const *environ, t_shell *shell)
{
    shell->options      = 0;
    shell->quit         = 0;
    shell->status       = 0;
    shell->progname     = ft_strrchr(progname, '/');
    shell->progname     = (shell->progname != NULL) ? shell->progname + 1 : progname;
    shell->bin_path     = ft_strdup("/usr/gnu/bin:/usr/local/bin:/usr/bin:/bin");
    shell->global_env   = ft_init_global_environ(shell->progname, environ);
    shell->internal_env = ft_init_internal_environ(shell->progname);
    ft_init_shell_prompt(&shell->prompt);
    ft_init_shell_command(&shell->command);
    shell->history = ft_init_shell_history(NULL); // TODO(gbo): parser le fichier d'historique de commandes
    ft_init_shell_highlight(&shell->highlighted);
    ft_init_shell_terminal(&shell->terminal);
    ft_memset(shell->sigs, -1, sizeof(shell->sigs));
}
