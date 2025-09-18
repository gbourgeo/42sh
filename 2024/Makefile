#**************************************************************************#
#                                                                          #
#                                                       :::      ::::::::  #
#  Makefile                                           :+:      :+:    :+:  #
#                                                   +:+ +:+         +:+    #
#  By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+       #
#                                               +#+#+#+#+#+   +#+          #
#  Created: 2013/12/28 02:12:40 by gbourgeo          #+#    #+#            #
#  Updated: 2018/04/06 19:02:35 by gbourgeo         ###   ########.fr      #
#                                                                          #
#**************************************************************************#

# Program
NAME = 42sh

# Directories
SRC_DIR         = srcs/
BUILTINS_DIR    = $(SRC_DIR)builtins/
COMMAND_DIR     = $(SRC_DIR)command/
HIGHLIGHT_DIR   = $(SRC_DIR)highlight/
HISTORY_DIR     = $(SRC_DIR)history/
KEYBOARD_DIR    = $(SRC_DIR)keyboard/
PIPEX_DIR       = $(SRC_DIR)pipex/
PROMPT_DIR      = $(SRC_DIR)prompt/
REDIRECTION_DIR = $(SRC_DIR)redirections/
SHELL_DIR       = $(SRC_DIR)shell/
TERMINAL_DIR    = $(SRC_DIR)terminal/

# Files
SRC = ft_check_and_exec.c            \
	ft_debug.c                       \
	ft_fork.c                        \
	ft_frees.c                       \
	ft_get_path.c                    \
	ft_log.c                         \
	ft_shell.c

BUILTINS_SRC = ft_cd.c               \
			ft_cd_check.c            \
			ft_cd_change_pwds.c      \
			ft_echo.c                \
			ft_env_main.c            \
			ft_env_option_parser.c   \
			ft_exit.c                \
			ft_getcwd.c              \
			ft_setenv.c              \
			ft_getenv.c              \
			ft_unsetenv.c

COMMAND_SRC = ft_command_highlight.c \
			ft_shell_command.c

HIGHLIGHT_SRC = ft_highlight_mode.c  \
				ft_highlight_paste.c \
				ft_highlight_yank.c

HISTORY_SRC = ft_shell_history.c

KEYBOARD_SRC = ft_key_analyser.c     \
			 ft_key_control.c        \
			 ft_key_esc.c            \
			 ft_key_history.c        \
			 ft_key_move.c           \
			 ft_key_suppr.c
#			 ft_key_shift.c

PROMPT_SRC = ft_shell_prompt.c

PIPEX_SRC = ft_pipe_left.c           \
			ft_pipe_pipe.c           \
			ft_pipe_rights.c         \
			ft_pipex_or_exec.c

REDIRECTION_SRC = ft_argsnew.c       \
		ft_lexical_analysis.c        \
		ft_token.c                   \
		ft_pipes_errors.c            \
		ft_separators_errors.c       \
		ft_syntax_analysis.c

SHELL_SRC = ft_shell_args_exec.c     \
			ft_shell_args_parse.c    \
			ft_shell_command_exec.c  \
			ft_shell_command_parse.c \
			ft_shell_exit.c          \
			ft_shell_init.c          \
			ft_shell_loop.c

TERMINAL_SRC = ft_shell_terminal.c

# Objects
OBJ_DIR = obj/
SRC_OBJ  = $(addprefix $(OBJ_DIR), $(SRC:.c=.o))
SRC_OBJ += $(addprefix $(OBJ_DIR), $(BUILTINS_SRC:.c=.o))
SRC_OBJ += $(addprefix $(OBJ_DIR), $(COMMAND_SRC:.c=.o))
SRC_OBJ += $(addprefix $(OBJ_DIR), $(HIGHLIGHT_SRC:.c=.o))
SRC_OBJ += $(addprefix $(OBJ_DIR), $(HISTORY_SRC:.c=.o))
SRC_OBJ += $(addprefix $(OBJ_DIR), $(KEYBOARD_SRC:.c=.o))
SRC_OBJ += $(addprefix $(OBJ_DIR), $(PIPEX_SRC:.c=.o))
SRC_OBJ += $(addprefix $(OBJ_DIR), $(PROMPT_SRC:.c=.o))
SRC_OBJ += $(addprefix $(OBJ_DIR), $(REDIRECTION_SRC:.c=.o))
SRC_OBJ += $(addprefix $(OBJ_DIR), $(SHELL_SRC:.c=.o))
SRC_OBJ += $(addprefix $(OBJ_DIR), $(TERMINAL_SRC:.c=.o))
SRC_OBJ += $(addprefix $(OBJ_DIR), $(LIBFT2_SRC:.c=.o))

# Compilation
CC = clang
CFLAGS = -Wall -Werror -Wextra # Recommended compiler warnings
CFLAGS += -Wpedantic -Wshadow -Wundef -Wconversion -Wunreachable-code
CFLAGS += -O2 # Recommended optimizations
LIBRAIRIES = -L$(LIBFT) -lft -ltermcap

# Dependencies
LIBFT = libft/
INCLUDES = -I includes -I $(LIBFT)includes

.PHONY: libft start end clean fclean re debug profile coverage scan

all: libft start $(NAME) end

.PHONY: libft
libft:
	@make -j -sC $(LIBFT)

.PHONY: start
start:
	@mkdir -p $(OBJ_DIR)
	@/bin/echo "[$(NAME)] Compilation flags: $(CFLAGS)"
	@/bin/echo -n "[$(NAME)] Compiling "

.PHONY: $(NAME)
$(NAME): $(SRC_OBJ)
	@$(CC) -o $@ $^ $(LIBRAIRIES)

# Include all .d files
-include $(DEPENDS)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@$(CC) $(CFLAGS) -MMD -o $@ -c $< $(INCLUDES) $(ENV)
	@/bin/echo -n "."

$(OBJ_DIR)%.o: $(BUILTINS_DIR)%.c
	@$(CC) $(CFLAGS) -MMD -o $@ -c $< $(INCLUDES) $(ENV)
	@/bin/echo -n "."

$(OBJ_DIR)%.o: $(COMMAND_DIR)%.c
	@$(CC) $(CFLAGS) -MMD -o $@ -c $< $(INCLUDES) $(ENV)
	@/bin/echo -n "."

$(OBJ_DIR)%.o: $(HIGHLIGHT_DIR)%.c
	@$(CC) $(CFLAGS) -MMD -o $@ -c $< $(INCLUDES) $(ENV)
	@/bin/echo -n "."

$(OBJ_DIR)%.o: $(HISTORY_DIR)%.c
	@$(CC) $(CFLAGS) -MMD -o $@ -c $< $(INCLUDES) $(ENV)
	@/bin/echo -n "."

$(OBJ_DIR)%.o: $(KEYBOARD_DIR)%.c
	@$(CC) $(CFLAGS) -MMD -o $@ -c $< $(INCLUDES) $(ENV)
	@/bin/echo -n "."

$(OBJ_DIR)%.o: $(PIPEX_DIR)%.c
	@$(CC) $(CFLAGS) -MMD -o $@ -c $< $(INCLUDES) $(ENV)
	@/bin/echo -n "."

$(OBJ_DIR)%.o: $(PROMPT_DIR)%.c
	@$(CC) $(CFLAGS) -MMD -o $@ -c $< $(INCLUDES) $(ENV)
	@/bin/echo -n "."

$(OBJ_DIR)%.o: $(REDIRECTION_DIR)%.c
	@$(CC) $(CFLAGS) -MMD -o $@ -c $< $(INCLUDES) $(ENV)
	@/bin/echo -n "."

$(OBJ_DIR)%.o: $(SHELL_DIR)%.c
	@$(CC) $(CFLAGS) -MMD -o $@ -c $< $(INCLUDES) $(ENV)
	@/bin/echo -n "."

$(OBJ_DIR)%.o: $(TERMINAL_DIR)%.c
	@$(CC) $(CFLAGS) -MMD -o $@ -c $< $(INCLUDES) $(ENV)
	@/bin/echo -n "."

.PHONY: end
end:
	@echo ""

.PHONY: clean
clean:
	@make -sC $(LIBFT) clean
	@/bin/rm -rf $(OBJ_DIR) *.gcov gmon.out
	@/bin/echo "[$(NAME)] Objects erased."

.PHONY: flcean
fclean: clean
	@make -sC $(LIBFT) fclean
	@/bin/rm -rf $(NAME) $(NAME).html
	@/bin/echo "[$(NAME)] Binary erased."

.PHONY: re
re: fclean all

.PHONY: debug
debug:
	@echo "[$(NAME)] Debug mode activated"
debug: CFLAGS = -Wall -Werror -Wextra -O0 -g3
debug: CFLAGS += -D_FORTIFY_SOURCE=2 # Run-time buffer overflow detection
debug: CFLAGS += -fasynchronous-unwind-tables # Increased reliability of backtraces
debug: CFLAGS += -fstack-clash-protection # Increased reliability of stack overflow detection
debug: re

.PHONY: profile
profile:
	@echo "[$(NAME)] Profiling mode activated"
profile: CFLAGS = -Wall -Werror -Wextra -O0 -g3 -pg
profile: LIBRAIRIES += -pg
profile: re

.PHONY: coverage
coverage:
	@echo "[$(NAME)] Coverage mode activated"
coverage: CFLAGS = -Wall -Werror -Wextra -O0 -g3 --coverage
coverage: LIBRAIRIES += --coverage
coverage: re

.PHONY: scan
scan: fclean
scan:
	@echo "[$(NAME)] Scan-build launched"
	scan-build -analyze-headers --force-analyze-debug-code -k \
		-enable-checker optin.core.EnumCastOutOfRange \
		-enable-checker optin.performance.Padding \
		-enable-checker optin.portability.UnixAPI \
		-enable-checker optin.taint.TaintedAlloc \
		-enable-checker security.ArrayBound \
		-enable-checker security.MmapWriteExec \
		-enable-checker security.PointerSub \
		-enable-checker security.PutenvStackArray \
		-enable-checker security.cert.env.InvalidPtr \
		-enable-checker security.insecureAPI.DeprecatedOrUnsafeBufferHandling \
		-enable-checker security.insecureAPI.strcpy \
		-enable-checker valist.CopyToSelf \
		-enable-checker valist.Uninitialized \
		-enable-checker valist.Unterminated \
		-enable-checker webkit.NoUncountedMemberChecker \
		-enable-checker webkit.RefCntblBaseVirtualDtor \
		-enable-checker webkit.UncountedLambdaCapturesChecker \
		make
