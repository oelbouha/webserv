#|	                     __          _
#|	   __  ___________ _/ /___ ___  (_)
#|	  / / / / ___/ __ `/ / __ `__ \/ /
#|	 / /_/ (__  ) /_/ / / / / / / / / 
#|	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
#|	/____/	User: Youssef Salmi
#|			File: Makefile 
#|


NAME := #1

CC := c++
CFLAGS := -Wall -Wextra -Werror -std=c++98

SRC := main.cpp 

OBJ := $(addprefix obj/,$(SRC:.cpp=.o))

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

obj/%.o: %.cpp
	@mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -rf obj/

fclean:
	rm -rf obj/ $(NAME)

re: fclean all

.PHONY: all clean fclean re

