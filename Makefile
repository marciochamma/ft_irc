# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fde-alen <fde-alen@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/27 23:19:17 by mchamma           #+#    #+#              #
#    Updated: 2025/04/12 21:37:14 by fde-alen         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv
CC = c++
FLAGS = -std=c++98 -Werror -Wextra -Wall

SRC_DIR = src/
OBJ_DIR = obj/
BONUS_OBJ_DIR = bonus_obj/

SRC = $(shell find src -name "*.cpp")
OBJ = $(patsubst $(SRC_DIR)%.cpp,$(OBJ_DIR)%.o,$(SRC))

INC = -I inc/

#--------------------//-------------------#

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) $(INC) $(LIB) -o $(NAME)

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	mkdir -p $(dir $@)
	$(CC) $(FLAGS) $(INC) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(BONUS_OBJ_DIR)
	clear

fclean: clean
	rm -f $(NAME)
	rm -f $(BONUS_NAME)
	clear

re: fclean all

r: fclean all
	make clean

do: r
	./ircserv 6667 pass

dov: r
	valgrind --leak-check=full ./ircserv 6667 pass

dol: r
	leaks --atExit --  ./ircserv 6667 pass

nc:
	clear
	nc localhost 6667

.PHONY: all clean fclean re r

# leaks --atExit -- ./program

#--------------------//-------------------#

.PHONY: all clean fclean re r


#--------------------//-------------------#
# Bonus rules
#--------------------//-------------------#

BONUS_NAME      = ircserv_bonus
BONUS_SRC_DIR   = src_bonus/
BONUS_OBJ_DIR   = bonus_obj/
BONUS_INC       = -I inc_bonus/

BONUS_SRC       = $(shell find $(BONUS_SRC_DIR) -name "*.cpp")
BONUS_OBJ       = $(patsubst $(BONUS_SRC_DIR)%.cpp,$(BONUS_OBJ_DIR)%.o,$(BONUS_SRC))

bonus: $(BONUS_NAME)

$(BONUS_NAME): $(BONUS_OBJ)
	$(CC) $(BONUS_OBJ) $(BONUS_INC) $(LIB) -o $(BONUS_NAME)

$(BONUS_OBJ_DIR)%.o: $(BONUS_SRC_DIR)%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(FLAGS) $(BONUS_INC) -c $< -o $@

cleanbonus:
	@rm -rf $(BONUS_OBJ_DIR)
	@echo "Bonus objects cleaned!"

fcleanbonus: cleanbonus
	@rm -f $(BONUS_NAME)
	@echo "Bonus executable cleaned!"

.PHONY: bonus cleanbonus fcleanbonus

#--------------------//-------------------#

# leaks --atExit -- ./program

# https://modern.ircdocs.horse/

# https://medium.com/@afatir.ahmedfatir/small-irc-server-ft-irc-42-network-7cee848de6f9

# https://github.com/AhmedFatir/ft_irc/tree/master

# The standard port for client-server connections is TCP/6667

# https://reactive.so/post/42-a-comprehensive-guide-to-ft_irc/

# valgrind --leak-check=full --show-leak-kinds=all nc localhost 6667

# https://github.com/alineagq/ft_irc

# https://ircv3.net/specs/extensions/capability-negotiation.html

# tests
# ctrl+d (com ctrl+d man ctrl+d d)
# ctrl+z (suspend client) and fg (foreground)
# ctrl+c and ctrl+\ - disconnect
# valgrind
# force client disconnect (x, ctrl+c and ctrl+\, /quit)
