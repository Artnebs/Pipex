# ------------------------------ VARIABLES ----------------------------------- #

# Program Names
NAME            = pipex
BONUS_NAME      = pipex_bonus

# Directories
SRC_DIR         = srcs
BONUS_DIR       = bonus
LIBFT_DIR       = Libft_GNL_Printf

# Libft Setup
LIBFT           = $(LIBFT_DIR)/libft.a
LIBFT_REPO      = https://github.com/Artnebs/Libft_GNL_Printf.git

# Compiler and Flags
CC              = gcc
CFLAGS          = -Wall -Wextra -Werror
INCLUDES        = -Iincludes -I$(LIBFT_DIR)/includes

# ---------------------------------------------------------------------------
# MANDATORY sources
#   main.c & main_helpers.c provide main() & handle_args() (only for mandatory).
#   The rest are your usual mandatory .c files that you ALSO need for bonus,
#   EXCEPT you don't want main.c/main_helpers.c for the bonus build.
# ---------------------------------------------------------------------------
MANDATORY_MAIN_SRCS = $(SRC_DIR)/main.c \
                      $(SRC_DIR)/main_helpers.c

MANDATORY_OTHER_SRCS = $(SRC_DIR)/parsing.c \
                       $(SRC_DIR)/execute.c \
                       $(SRC_DIR)/helpers.c \
                       $(SRC_DIR)/helpers2.c \
                       $(SRC_DIR)/redirection.c

MANDATORY_SRCS       = $(MANDATORY_MAIN_SRCS) $(MANDATORY_OTHER_SRCS)
MANDATORY_OBJS       = $(MANDATORY_SRCS:.c=.o)

# ---------------------------------------------------------------------------
# BONUS sources
#   main_bonus.c & main_helpers_bonus.c provide the bonus main() & handle_args_bonus().
#   We STILL want to reuse the "other" mandatory .c files, but skip main.c/main_helpers.c.
# ---------------------------------------------------------------------------
BONUS_ONLY_SRCS = $(BONUS_DIR)/main_bonus.c \
                  $(BONUS_DIR)/main_helpers_bonus.c \
                  $(BONUS_DIR)/here_doc_bonus.c \
                  $(BONUS_DIR)/here_doc_helpers.c \
                  $(BONUS_DIR)/multiple_pipes_bonus.c \
                  $(BONUS_DIR)/multiple_pipes_helpers_bonus.c \
				  $(BONUS_DIR)/multiple_pipes_utils_bonus.c

# We reuse MANDATORY_OTHER_SRCS as well to get the same logic for parsing, execute, etc.
BONUS_SRCS      = $(MANDATORY_OTHER_SRCS) $(BONUS_ONLY_SRCS)
BONUS_OBJS      = $(BONUS_SRCS:.c=.o)

# ------------------------------ TARGETS ------------------------------------- #

all: $(LIBFT) $(NAME)

# Build the mandatory program with *all* mandatory sources (including main.c, main_helpers.c).
$(NAME): $(MANDATORY_OBJS)
	@echo "Linking $(NAME)..."
	$(CC) $(CFLAGS) $(INCLUDES) $(MANDATORY_OBJS) -L$(LIBFT_DIR) -lft -o $(NAME)
	@echo "Build complete!"

# Build the bonus program with:
#   - MANDATORY_OTHER_SRCS (no main.c / main_helpers.c)
#   - BONUS_ONLY_SRCS
bonus: $(LIBFT) $(BONUS_OBJS)
	@echo "Linking (BONUS) $(BONUS_NAME)..."
	$(CC) $(CFLAGS) $(INCLUDES) $(BONUS_OBJS) -L$(LIBFT_DIR) -lft -o $(BONUS_NAME)
	@echo "Bonus build complete!"

# Compile object files for MANDATORY (main + others)
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Compile object files for BONUS
$(BONUS_DIR)/%.o: $(BONUS_DIR)/%.c
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Build Libft if not present
$(LIBFT):
	@if [ ! -d "$(LIBFT_DIR)" ]; then \
		echo "Cloning Libft repository..."; \
		git clone $(LIBFT_REPO) $(LIBFT_DIR); \
	fi
	@echo "Building Libft library..."
	@$(MAKE) -C $(LIBFT_DIR)

# Clean object files
clean:
	@echo "Cleaning object files..."
	rm -f $(MANDATORY_OBJS) $(BONUS_OBJS)
	@if [ -d "$(LIBFT_DIR)" ]; then \
		$(MAKE) -C $(LIBFT_DIR) clean; \
	fi

# Full clean, removing program and additional files
fclean: clean
	@echo "Removing $(NAME) and $(BONUS_NAME) binaries..."
	rm -f $(NAME) $(BONUS_NAME)
	@if [ -d "test_files" ]; then \
		echo "Removing test_files/ directory..."; \
		rm -rf test_files; \
	fi
	@echo "Removing Libft directory (optional)..."
	rm -rf $(LIBFT_DIR)

# Rebuild everything
re: fclean all

.PHONY: all clean fclean re bonus
