/****************************************************************************/
/*                                                                          */
/*                                                       :::      ::::::::  */
/*  ft_shell.h                                         :+:      :+:    :+:  */
/*                                                   +:+ +:+         +:+    */
/*  By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+       */
/*                                               +#+#+#+#+#+   +#+          */
/*  Created: 2013/12/28 04:49:32 by gbourgeo          #+#    #+#            */
/*  Updated: 2018/04/06 17:34:07 by gbourgeo         ###   ########.fr      */
/*                                                                          */
/****************************************************************************/

#ifndef FT_MINISHELL_H
#define FT_MINISHELL_H

#include "ft_defines.h"
#include "ft_shell_command.h"
#include "ft_shell_constants.h"
#include "ft_shell_environ.h"
#include "ft_shell_history.h"
#include "ft_shell_prompt.h"
#include "ft_shell_terminal.h"
#include <stddef.h>
#include <stdint.h>
#include <sys/signal.h>
#include <sys/types.h>

#define SHELL_VERSION_MAJOR 1
#define SHELL_VERSION_MINOR 0
#define SHELL_VERSION_PATCH 0

#define SEPARATORS          "&|;"
#define PIPES               "<>|"
#define DOUBLE_OR           1
#define DOUBLE_AND          2
#define PIPE_LEFT           3
#define PIPE_RIGHT          4
#define PIPE_RIGHT_2        5
#define PIPE_PIPE           6

#ifdef __linux__
# define sighandler_t __sighandler_t
# define NSIG         _NSIG
#elif __APPLE__ || __MACH__
# define sighandler_t sig_t
# define NSIG         __DARWIN_NSIG
#endif

typedef enum shell_command_save_e
{
    SHELL_COMMAND_SAVE = 0,
    SHELL_COMMAND_DONT_SAVE,
} e_shcmdsave;

/******************************************************************************
 * SHELL STRUCTURE
 *****************************************************************************/

typedef struct _align(128) s_shell
{
    t_term       terminal;     /* Informations du terminal */
    t_prompt     prompt;       /* Prompt du Shell */
    t_hist       history;      /* Informations d'Historique */
    t_cmd       *command;      /* Historique de commandes et commande actuelle en tête */
    uint8_t     *yank;         /* Copie de la (des) dernière(s) zone(s) de texte surlignée */
    const char  *progname;     /* Nom du programme */
    t_env        environ;      /* Environnement du Shell */
    sighandler_t sigs[NSIG];   /* Tableau de sauvegarde des handlers des signaux */
    uint32_t     options;      /* Options du Shell */
    int          fd;           /* File Descriptor de lecture des données */
    int          quit;         /* Valeur définissant si le programme doit se terminer */
    int          status;       /* Valeur de sortie de la dernière commande exécutée */
} t_shell;

/******************************************************************************
 * SHELL FUNCTIONS
 *****************************************************************************/

/**
 * @brief Fonction main du Shell.
 * @param argc Nombre d'arguments du programme
 * @param argv Liste des arguments du programme
 * @return 0 si OK.
 */
int main(int argc, const char **argv);

/**
 * @brief Fonction d'exécution des arguments du Shell.
 * @param argv Tableau des arguments
 * @param shell Structure du Shell
 */
void ft_shell_args_exec(const char **argv, t_shell *shell);

/**
 * @brief Fonction de parsing des options du Shell.
 * @param argv Liste des arguments du programme
 * @param shell Structure du Shell
 */
const char **ft_shell_args_parse(const char **argv, t_shell *shell);

/**
 * @brief Execution de la commande en cours.
 * @param[in] shell Structure du shell
 */
void ft_shell_exec_command(t_shell *shell);

/**
 * @brief Fonction de suppression de la structure du shell
 * @param shell Structure du shell
 */
void ft_shell_exit(t_shell *shell);

/**
 * @brief Parse les arguments du programme et initialise les variables et
 * l'environnement du Shell.
 * @param progname Nom du programme
 * @param environ Environnement du programme
 * @param shell Structure du Shell
 */
void ft_shell_init(const char *progname, const char **environ,
                   t_shell *shell);

/**
 * @brief Réinitialise le Shell si en mode intéractif :
 * * Sauvegarde la Commande dans l'Historique (suivant l'option),
 * * Réinitialise les Zones de textes surlignés,
 * * Réinitialise le Prompt,
 * * Réinitialise les modes du terminal,
 * * Réinitialise la Commande.
 * @param shell Structure du Shell
 * @param savecmd Option de sauvegarde de la Commande dans l'Historique
 */
void ft_shell_interactive_reinit(t_shell *shell, e_shcmdsave savecmd);

/**
 * @brief Boucle d'execution du Shell.
 * @param shell Structure du shell
 */
void ft_shell_loop(t_shell *shell);

/**
 * @brief Cette fonction reçoit en entrée une chaîne de caractères représentant
 * un chemin absolu pouvant comporter des '..' et des '.' (ex: "/home/user/work/../libft/./src").
 * Elle parse cette chaîne et traduit les '..' et '.' pour en sortir une chaine sans (ex:
 * "/home/user/libft/src").
 * @param[in] path Chemin absolu commençant par un '/'
 * @return Chemin absolu sans '..' ni '.'
 */
char *ft_shell_resolve_path(char *path);

int       check_and_exec(const char **args, t_shell *shell);
retcode_e fork_function(const char **args, t_shell *shell);
char     *ft_get_path(char *pwd);

/*****************************************************************************
 * A SUPPRIMER
 ******************************************************************************/
// void ft_free_list(t_args **list);
// char   *ft_lexical_analysis(char *command);
// int     ft_syntax_analysis(char **args);
// t_args *ft_argsnew(char **args, int type);
// void ft_pipex_or_exec(t_args *list, t_shell *shell);
// void ft_pipe_pipe(t_args *pipes, t_shell *shell);
// void ft_pipe_right(t_args *pipes, t_shell *shell);
// void ft_pipe_right_2(t_args *pipes, t_shell *shell);
// void ft_pipe_left(t_args *pipes, t_shell *shell);

#endif
