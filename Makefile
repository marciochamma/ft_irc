# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mchamma <mchamma@student.42sp.org.br>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/27 23:19:17 by mchamma           #+#    #+#              #
#    Updated: 2025/02/11 01:08:13 by mchamma          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv
CC = c++
FLAGS = -std=c++98 -Werror -Wextra -Wall

SRC_DIR = src/
OBJ_DIR = obj/

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
	clear

fclean: clean
	rm -f $(NAME)
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