# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+           #
#    Created: 2024/12/04 10:28:35 by anebbou           #+#    #+#              #
#    Updated: 2025/01/18 13:45:00 by anebbou          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Program Name
NAME = pipex

# Libft Setup
LIBFT_DIR = Libft_GNL_Printf
LIBFT = $(LIBFT_DIR)/libft.a
LIBFT_REPO = https://github.com/Artnebs/Libft_GNL_Printf.git

# Source Files
SRCS = redirection.c parsing.c multiple_pipes.c main.c here_doc.c execute.c helpers.c
OBJS = $(SRCS:.c=.o)

# Compiler and Flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -I. -I$(LIBFT_DIR)/includes

# Default Rule
all: $(LIBFT) $(NAME)

# Build the main executable
$(NAME): $(OBJS)
	@echo "Linking $(NAME)..."
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -L$(LIBFT_DIR) -lft

# Compile object files
%.o: %.c
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Build Libft
$(LIBFT):
	@if [ ! -d "$(LIBFT_DIR)" ]; then \
		echo "Cloning Libft repository..."; \
		git clone $(LIBFT_REPO) $(LIBFT_DIR); \
	fi
	@echo "Building Libft library..."
	@make -C $(LIBFT_DIR) all

# Clean object files
clean:
	@echo "Cleaning object files..."
	rm -f $(OBJS)
	@if [ -d "$(LIBFT_DIR)" ]; then \
		make -C $(LIBFT_DIR) clean; \
	fi

# Full clean, removing Libft, .txt files, and test_files folder
fclean: clean
	@echo "Cleaning $(NAME), .txt files, and test_files directory..."
	rm -f $(NAME) *.txt
	@if [ -d "$(LIBFT_DIR)" ]; then \
		echo "Removing Libft directory..."; \
		rm -rf $(LIBFT_DIR); \
	fi
	@if [ -d "test_files" ]; then \
		echo "Removing test_files directory..."; \
		rm -rf test_files; \
	fi

# Rebuild everything
re: fclean all

# Ensure makefile targets are not interpreted as filenames
.PHONY: all clean fclean re
