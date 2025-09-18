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
#include "ft_shell_history.h"
#include "ft_shell_prompt.h"
#include "ft_shell_terminal.h"
#include "ft_shell_token.h"
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

/******************************************************************************
 * SHELL STRUCTURE
 *****************************************************************************/

typedef struct _align(128) s_shell
{
    t_term       terminal;     /* Informations du terminal */
    t_prompt     prompt;       /* Prompt du shell */
    t_hist       history;      /* Informations d'historique */
    t_cmd       *command;      /* Historique de commandes et commande actuelle en tête */
    size_t       command_size; /* Taille de l'historique de commandes */
    uint8_t     *yank;         /* Copie */
    const char  *progname;     /* Nom du programme */
    const char  *bin_path;     /* Chemins absolu des binaires */
    char       **global_env;   /* Variables d'environnement globales */
    char       **internal_env; /* Variables d'environnement internes */
    sighandler_t sigs[NSIG];   /* Tableau de sauvegarde des handlers des signaux */
    uint32_t     options;      /* Options du shell */
    int          quit;         /* Valeur définissant si le programme doit se terminer */
    int          status;       /* Valeur de sortie de la dernière commande exécutée */
} t_shell;

/******************************************************************************
 * SHELL FUNCTIONS
 *****************************************************************************/

/**
 * @brief Fonction main du shell.
 * @param argc Nombre d'arguments du programme
 * @param argv Liste des arguments du programme
 * @return 0 si OK.
 */
int main(int argc, const char **argv);

/**
 * @brief Fonction d'exécution des arguments du Shell.
 * @param argv Tableau des arguments
 * @param shell Structure interne du Shell
 */
void ft_shell_args_exec(const char **argv, t_shell *shell);

/**
 * @brief Fonction de parsing des options du Shell.
 * @param argv Liste des arguments du programme
 * @param shell Structure interne du Shell
 */
const char **ft_shell_args_parse(const char **argv, t_shell *shell);

/**
 * @brief Execution de la commande en cours.
 * @param[in] shell Structure interne du shell
 */
void ft_shell_command_exec(t_shell *shell);

/**
 * @brief Parsing de la commande en cours.
 * @param shell Structure interne du shell
 */
void ft_shell_command_parse(t_shell *shell);

/**
 * @brief Fonction de suppression de la structure interne du shell
 * @param shell Structure interne du shell
 */
void ft_shell_exit(t_shell *shell);

/**
 * @brief Parse les arguments du programme et initialise les variables et
 * l'environnement du Shell.
 * @param progname Nom du programme
 * @param environ Environnement du programme
 * @param shell Structure interne du Shell
 */
void ft_shell_init(const char *progname, const char **environ,
                   t_shell *shell);

/**
 * @brief Boucle d'execution du Shell.
 * @param shell Structure interne du shell
 */
void ft_shell_loop(t_shell *shell);

/**
 * @brief Fonction de deboggage du Shell. Affiche les informations suivantes:
 *
 * - 1ère ligne: Ligne (actuelle/totale), Colonne (actuelle/totale), highlight
 * (activé/désactivé), Position du curseur dans la ligne de commande, Longueur
 * du Prompt.
 *
 * - 2nd ligne : Si non NULL, affiche les caractères du buffer passé en
 * paramètre au format hexadécimal.
 *
 * - 3ème ligne: Affiche le zones de textes surlignées.
 *
 * - 4ème ligne: Affiche la ligne de commande courante du Shell.
 *
 * - 5ème ligne: Affiche la ligne de commande sauvegardée.
 *
 * - 6ème ligne: Affiche l'historique de commandes sur une seule ligne.
 *
 * @param buf Buffer à afficher
 * @param ret Taille du buffer
 * @param shell Structure interne du Shell
 */
void debug_command_line(const uint8_t *buf, long ret, t_shell *shell);

/**
 * @brief Fonction de deboggage des tokens après parsing d'une ligne de
 * commande.
 * @param token Liste de token
 * @param shell Structure interne du shell
 */
void debug_tokens(t_token *token, t_shell *shell);

int       check_and_exec(const char **args, t_shell *shell);
retcode_e fork_function(const char **args, t_shell *shell);
char     *ft_get_path(char *pwd);

/*****************************************************************************
 * FREE
 ******************************************************************************/

void ft_free_list(t_args **list);

/*****************************************************************************
 * PARSER
 ******************************************************************************/

char   *ft_lexical_analysis(char *command);
int     ft_syntax_analysis(char **args);
t_args *ft_argsnew(char **args, int type);

/*****************************************************************************
 * PIPEX
 ******************************************************************************/

void ft_pipex_or_exec(t_args *list, t_shell *shell);
void ft_pipe_pipe(t_args *pipes, t_shell *shell);
void ft_pipe_right(t_args *pipes, t_shell *shell);
void ft_pipe_right_2(t_args *pipes, t_shell *shell);
void ft_pipe_left(t_args *pipes, t_shell *shell);

#endif
