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

#include "ft_command.h"
#include "ft_constants.h"
#include "ft_highlight.h"
#include "ft_history.h"
#include "ft_prompt.h"
#include "ft_termios.h"
#include "ft_token.h"
// #include <stdint.h>
#include <sys/signal.h>
#include <sys/types.h>

#define SHELL_VERSION_MAJOR    1
#define SHELL_VERSION_MINOR    0
#define SHELL_VERSION_PATCH    0

#define SEPARATORS             "&|;"
#define PIPES                  "<>|"
#define DOUBLE_OR              1
#define DOUBLE_AND             2
#define PIPE_LEFT              3
#define PIPE_RIGHT             4
#define PIPE_RIGHT_2           5
#define PIPE_PIPE              6
#define PRINTABLE_KEYS(i, len) (buf[i] >= 32 && buf[i] <= 126)
#define CMD_SIZE               10

#ifdef __linux__
# define sighandler_t __sighandler_t
# define NSIG         _NSIG
#elif __APPLE__ || __MACH__
# define sighandler_t sig_t
# define NSIG         __DARWIN_NSIG
#endif

enum
{
    SHELL_TERMATTR_LOADED  = 0x01, // 2^0
    SHELL_DEBUG_MODE       = 0x02, // 2^1
    SHELL_INTERACTIVE_MODE = 0x04, // 2^2
};

typedef struct __attribute__((aligned(LARGE_ALIGNMENT_CONSTANT))) s_shell
{
    unsigned int options;      /* Options du shell */
    int          quit;         /* Valeur définissant si le programme doit se terminer */
    int          status;       /* Valeur de sortie de la dernière commande exécutée */
    const char  *progname;     /* Nom du programme */
    const char  *bin_path;     /* Chemins absolu des binaires */
    char       **global_env;   /* Variables d'environnement globales */
    char       **internal_env; /* Variables d'environnement internes */
    t_prompt     prompt;       /* Structure du prompt */
    t_cmd        command;      /* Structure Commande actuelle */
    t_hist      *history;      /* Structure Historique des commandes */
    t_hlight     highlighted;  /* Structure Texte surligné (pour la copie) */
    t_term       terminal;     /* Structure du terminal */
    sighandler_t sigs[NSIG];   /* Tableau de sauvegarde des handlers des signaux */
} t_shell;

/******************************************************************************
 * SHELL FUNCTIONS
 *****************************************************************************/
int          main(int argc, const char **argv);

/**
 * @brief Parse les arguments du programme et initialise les variables et
 * l'environnement du Shell.
 * @param progname Nom du programme
 * @param environ Environnement du programme
 * @param shell Structure interne du Shell
 */
void         ft_shell_init(const char *progname, char         *const *environ,
                           t_shell *shell);

/**
 * @brief Fonction de parsing des options du Shell.
 * @param argv Tableau des arguments
 * @param shell Structure interne du Shell
 */
const char **ft_shell_options(const char **argv, t_shell *shell);

/**
 * @brief Execute le Shell.
 * @param av Arguments
 * @param shell Structure interne du shell
 */
void         ft_shell_exec(const char **argv, t_shell *shell);

/**
 * @brief Fonction de destruction de la structure interne du shell.
 * @param shell Structure interne du shell
 */
void         ft_shell_exit(t_shell *shell);

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
void         debug_command_line(const char *buf, ssize_t ret, t_shell *shell);

/**
 * @brief Fonction de deboggage des tokens après parsing d'une ligne de
 * commande.
 * @param token Liste de token
 * @param shell Structure interne du shell
 */
void         debug_tokens(t_token *token, t_shell *shell);

int          check_and_exec(const char **args, t_shell *shell);
int          fork_function(const char **args, t_shell *shell);
char        *ft_get_path(char *pwd);

/*****************************************************************************
 * PROMPT FUNCTIONS
 ******************************************************************************/
/**
 * @brief Affiche le prompt du shell (mode interactif).
 * @param shell Structure interne du shell
 */
void         ft_shell_prompt(t_shell *shell);

/*****************************************************************************
 * FREE
 ******************************************************************************/

void         ft_free_list(t_args **list);

/*****************************************************************************
 * PARSER
 ******************************************************************************/

void         ft_parse_command(t_shell *shell);
char        *ft_lexical_analysis(char *command);
int          ft_syntax_analysis(char **args);
t_args      *ft_argsnew(char **args, int type);

/*****************************************************************************
 * PIPEX
 ******************************************************************************/

void         ft_pipex_or_exec(t_args *list, t_shell *shell);
void         ft_pipe_pipe(t_args *pipes, t_shell *shell);
void         ft_pipe_right(t_args *pipes, t_shell *shell);
void         ft_pipe_right_2(t_args *pipes, t_shell *shell);
void         ft_pipe_left(t_args *pipes, t_shell *shell);

#endif
