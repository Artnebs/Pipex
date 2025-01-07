# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+           #
#    Created: 2024/12/04 10:28:35 by anebbou           #+#    #+#              #
#    Updated: 2024/12/24 00:27:39 by anebbou          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Program Names
NAME = 
BONUS = 

# Libft Setup
LIBFT_DIR = Libft_GNL_Printf
LIBFT = $(LIBFT_DIR)/libft.a
LIBFT_REPO = https://github.com/Artnebs/Libft_GNL_Printf.git

# Source Files for push_swap
SRCS = 
OBJS = $(SRCS:.c=.o)

# Source Files for checker (bonus)
BONUS_SRCS = 
BONUS_OBJS = $(BONUS_SRCS:.c=.o)

# Compiler and Flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -I. -I$(LIBFT_DIR)/includes

# Silence unnecessary make outputs
MAKEFLAGS += --silent

# Default Rule
all: $(LIBFT) $(NAME)

# Build push_swap executable
$(NAME): $(OBJS)
	@echo "Linking $(NAME)..."
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -L$(LIBFT_DIR) -lft

# Build checker (bonus) executable
bonus: $(LIBFT) $(BONUS)

$(BONUS): $(BONUS_OBJS)
	@echo "Linking $(BONUS)..."
	$(CC) $(CFLAGS) $(BONUS_OBJS) -o $(BONUS) -L$(LIBFT_DIR) -lft

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
	@make -C $(LIBFT_DIR)

# Clean object files
clean:
	@echo "Cleaning object files..."
	rm -f $(OBJS) $(BONUS_OBJS)
	@if [ -d "$(LIBFT_DIR)" ]; then \
		make -C $(LIBFT_DIR) clean; \
	fi

# Clean all files
fclean: clean
	@echo "Cleaning $(NAME), $(BONUS), and Libft..."
	rm -f $(NAME) $(BONUS)
	@if [ -d "$(LIBFT_DIR)" ]; then \
		rm -rf $(LIBFT_DIR); \
		echo "Deleted Libft_GNL_Printf directory."; \
	fi

# Rebuild everything
re: fclean all

# Ensure makefile targets are not interpreted as filenames
.PHONY: all clean fclean re bonus
