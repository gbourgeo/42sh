#******************************************************************************#
#																			  #
#														 :::	  ::::::::	#
#	Makefile										   :+:	  :+:	:+:	#
#													 +:+ +:+		 +:+	  #
#	By: gbourgeo <gbourgeo@student.42.fr>		  +#+  +:+	   +#+		 #
#												 +#+#+#+#+#+   +#+			#
#	Created: 2013/12/28 02:12:40 by gbourgeo		  #+#	#+#			  #
#	Updated: 2018/04/06 19:02:35 by gbourgeo		 ###   ########.fr		#
#																			  #
#******************************************************************************#

NAME	= 42sh

OBJ_DIR = obj/

SRC_DIR = srcs/
SRC	 = ft_shell.c				\
		ft_debug.c				\
		ft_log.c				\
		ft_fork.c				\
		ft_frees.c				\
		ft_get_path.c			\
		ft_history.c			\
		ft_check_and_exec.c		\
		ft_termios_functions.c
SRC_OBJ = $(addprefix $(OBJ_DIR), $(SRC:.c=.o))

BUILTINS_DIR = $(SRC_DIR)builtins/
BUILTINS_SRC = ft_cd.c			\
			ft_cd_check.c		\
			ft_cd_change_pwds.c	\
			ft_echo.c			\
			ft_env.c			\
			ft_exit.c			\
			ft_getcwd.c			\
			ft_getenv.c			\
			ft_setenv.c			\
			ft_unsetenv.c
SRC_OBJ += $(addprefix $(OBJ_DIR), $(BUILTINS_SRC:.c=.o))

COMMAND_DIR = $(SRC_DIR)command/
COMMAND_SRC = ft_command.c
SRC_OBJ += $(addprefix $(OBJ_DIR), $(COMMAND_SRC:.c=.o))

HIGHLIGHT_DIR = $(SRC_DIR)highlight/
HIGHLIGHT_SRC = ft_highlight.c			\
				ft_highlight_mode.c		\
				ft_highlight_paste.c	\
				ft_highlight_yank.c
SRC_OBJ += $(addprefix $(OBJ_DIR), $(HIGHLIGHT_SRC:.c=.o))

HISTORY_DIR = $(SRC_DIR)history/
HISTORY_SRC = ft_history.c
SRC_OBJ += $(addprefix $(OBJ_DIR), $(HISTORY_SRC:.c=.o))

KEYBOARD_DIR = $(SRC_DIR)keyboard/
KEYBOARD_SRC = ft_key_analyser.c	\
			 ft_key_control.c		\
			 ft_key_history.c		\
			 ft_key_move.c			\
			 ft_key_print.c			\
			 ft_key_suppr.c
#			 ft_key_shift.c
SRC_OBJ += $(addprefix $(OBJ_DIR), $(KEYBOARD_SRC:.c=.o))

PIPEX_DIR = $(SRC_DIR)pipex/
PIPEX_SRC = ft_pipe_left.c		\
			ft_pipe_pipe.c		\
			ft_pipe_rights.c	\
			ft_pipex_or_exec.c
SRC_OBJ += $(addprefix $(OBJ_DIR), $(PIPEX_SRC:.c=.o))

REDIRECTION_DIR = $(SRC_DIR)redirections/
REDIRECTION_SRC = ft_argsnew.c	\
		ft_lexical_analysis.c	\
		ft_parser.c				\
		ft_token.c				\
		ft_pipes_errors.c		\
		ft_separators_errors.c	\
		ft_syntax_analysis.c
SRC_OBJ += $(addprefix $(OBJ_DIR), $(REDIRECTION_SRC:.c=.o))

SHELL_DIR = $(SRC_DIR)shell/
SHELL_SRC = ft_shell_exec.c		\
			ft_shell_exit.c		\
			ft_shell_init.c		\
			ft_shell_options.c	\
			ft_shell_prompt.c
SRC_OBJ += $(addprefix $(OBJ_DIR), $(SHELL_SRC:.c=.o))

TERMINAL_DIR = $(SRC_DIR)terminal/
TERMINAL_SRC = ft_shell_exec.c	\
			ft_shell_exit.c		\
			ft_shell_init.c		\
			ft_shell_options.c	\
			ft_shell_prompt.c
SRC_OBJ += $(addprefix $(OBJ_DIR), $(TERMINAL_SRC:.c=.o))

LIBFT2_DIR = libft2/
LIBFT2_SRC = ft_dup2.c
SRC_OBJ += $(addprefix $(OBJ_DIR), $(LIBFT2_SRC:.c=.o))

CC = gcc
CFLAGS	= -Wall -Werror -Wextra

LIBFT	= libft/

INCLUDES = -I includes -I $(LIBFT)includes

LIBRAIRIES = -L$(LIBFT) -lft -ltermcap

.PHONY: libft shell clean fclean re debug profile coverage

all: libft shell $(NAME)

libft:
	@make -j -sC $(LIBFT)

shell:
	@echo '[Compilation du 42Sh]'
	@mkdir -p $(OBJ_DIR)

$(NAME): $(SRC_OBJ)
	@$(CC) -o $@ $^ $(LIBRAIRIES)
	@echo ' > $@'

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@/bin/echo -n "."
	@$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

$(OBJ_DIR)%.o: $(BUILTINS_DIR)%.c
	@/bin/echo -n "."
	@$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

$(OBJ_DIR)%.o: $(COMMAND_DIR)%.c
	@/bin/echo -n "."
	@$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

$(OBJ_DIR)%.o: $(HIGHLIGHT_DIR)%.c
	@/bin/echo -n "."
	@$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

$(OBJ_DIR)%.o: $(HISTORY_DIR)%.c
	@/bin/echo -n "."
	@$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

$(OBJ_DIR)%.o: $(KEYBOARD_DIR)%.c
	@/bin/echo -n "."
	@$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

$(OBJ_DIR)%.o: $(PIPEX_DIR)%.c
	@/bin/echo -n "."
	@$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

$(OBJ_DIR)%.o: $(REDIRECTION_DIR)%.c
	@/bin/echo -n "."
	@$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

$(OBJ_DIR)%.o: $(SHELL_DIR)%.c
	@/bin/echo -n "."
	@$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

$(OBJ_DIR)%.o: $(TERMINAL_DIR)%.c
	@/bin/echo -n "."
	@$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

$(OBJ_DIR)%.o: $(LIBFT2_DIR)%.c
	@/bin/echo -n "."
	@$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

clean:
	@make -sC $(LIBFT) clean
	/bin/rm -rf $(OBJ_DIR) *.gcov gmon.out

fclean: clean
	@make -sC $(LIBFT) fclean
	/bin/rm -rf $(NAME) 42sh.html

re: fclean all

debug:
	@echo "[Debug mode enabled]"
debug: CFLAGS += -g3
debug: re

profile:
	@echo "[Profiling mode enabled]"
profile: CFLAGS += -g -pg
profile: LIBRAIRIES += -pg
profile: re

coverage:
	@echo "[Coverage mode enabled]"
coverage: CFLAGS += -O0 -g --coverage
coverage: LIBRAIRIES += --coverage
coverage: re
