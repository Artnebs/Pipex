# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/09 17:20:13 by anebbou           #+#    #+#              #
#    Updated: 2025/02/14 11:29:37 by anebbou          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ------------------------------ VARIABLES ----------------------------------- #

# Program Names
NAME            = pipex
NAME_BONUS      = pipex_bonus

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

# Mandatory Sources
SRCS_MAND       = $(SRC_DIR)/main.c \
                  $(SRC_DIR)/parsing.c \
                  $(SRC_DIR)/execute.c \
                  $(SRC_DIR)/helpers.c \
                  $(SRC_DIR)/helpers2.c \
                  $(SRC_DIR)/redirection.c

OBJS_MAND       = $(SRCS_MAND:.c=.o)

# Bonus Sources (Only compiled in bonus mode)
SRCS_BONUS      = $(BONUS_DIR)/here_doc_bonus.c \
                  $(BONUS_DIR)/multiple_pipes_bonus.c \
				  $(BONUS_DIR)/multiple_pipes_helpers_bonus.c \

OBJS_BONUS      = $(SRCS_BONUS:.c=.o)

# ------------------------------ TARGETS ------------------------------------- #

# Default Rule: build mandatory
all: $(LIBFT) $(NAME)

# Build the main executable from mandatory object files
$(NAME): $(OBJS_MAND)
	@echo "Linking $(NAME)..."
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJS_MAND) -L$(LIBFT_DIR) -lft -o $(NAME)
	@echo "Build complete!"

# Bonus Rule: build both mandatory and bonus separately
bonus: $(LIBFT) $(NAME_BONUS)

$(NAME_BONUS): $(OBJS_MAND) $(OBJS_BONUS)
	@echo "Linking $(NAME_BONUS) (with bonus)..."
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJS_MAND) $(OBJS_BONUS) \
		-L$(LIBFT_DIR) -lft -o $(NAME_BONUS)
	@echo "Build complete (with bonus)!"

# Compile object files for mandatory
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Compile object files for bonus
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
	rm -f $(OBJS_MAND) $(OBJS_BONUS)
	@if [ -d "$(LIBFT_DIR)" ]; then \
		$(MAKE) -C $(LIBFT_DIR) clean; \
	fi

# Full clean, removing program and additional files
fclean: clean
	@echo "Removing $(NAME) and $(NAME_BONUS) binaries..."
	rm -f $(NAME) $(NAME_BONUS)
	@if [ -d "test_files" ]; then \
		echo "Removing test_files/ directory..."; \
		rm -rf test_files; \
	fi
	@echo "Removing Libft directory (optional)..."
	rm -rf $(LIBFT_DIR)

# Rebuild everything
re: fclean all

# Ensure makefile targets are not interpreted as files
.PHONY: all clean fclean re bonus
