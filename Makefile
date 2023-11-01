#|	                     __          _
#|	   __  ___________ _/ /___ ___  (_)
#|	  / / / / ___/ __ `/ / __ `__ \/ /
#|	 / /_/ (__  ) /_/ / / / / / / / / 
#|	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
#|	/____/	User: Youssef Salmi
#|			File: Makefile 
#|


NAME := webserv

CC := c++
CFLAGS := -Wall -Wextra -Werror -std=c++98 -I./include -I./include/interfaces

SRC := main.cpp \
	Logger.cpp

OBJ := $(addprefix obj/,$(SRC:.cpp=.o))

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

obj/%.o: src/%.cpp
	@mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -rf obj/

fclean:
	rm -rf obj/ $(NAME)

re: fclean all

.PHONY: all clean fclean re

