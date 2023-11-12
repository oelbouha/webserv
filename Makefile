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

I := -I. \
	-I./include \
	-I./include/interfaces \
	-I./exceptions

CPPFLAGS := -Wall -Wextra -Werror -std=c++98 $(I)


BASE := Config.cpp \
	ConfigHelper.cpp \
	ConfigParser.cpp \
	ConfigParserFactory.cpp

SRC := main.cpp \
	$(BASE)

BASE_OBJ := $(addprefix obj/,$(BASE:.cpp=.o))
OBJ := $(addprefix obj/,$(SRC:.cpp=.o))

define template
.PHONY: $(1)
$(1):
	@:
endef

ifeq (test,$(firstword $(MAKECMDGOALS)))
  TEST_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  $(foreach arg,$(TEST_ARGS),$(eval $(call template,$(arg))))
endif

all: $(NAME)

test: $(BASE_OBJ) tests/test$(TEST_ARGS).cpp tests/test.h
	@$(CC) $(CPPFLAGS) -o test $(BASE_OBJ) tests/test$(TEST_ARGS).cpp
	@./test

$(NAME): $(OBJ) #obj/main.o obj/Config.o obj/ConfigHelper.o obj/ConfigParser.o obj/ConfigParserFactory.o
	$(CC) $(CPPFLAGS) -o $@ $^

.PHONY: echo
echo:
	@echo $(OBJ)

obj/%.o: src/%.cpp
	@mkdir -p $(shell dirname $@)
	$(CC) $(CPPFLAGS) -o $@ -c $<



clean:
	rm -rf obj/

fclean:
	rm -rf obj/ $(NAME)

re: fclean all

.PHONY: all clean fclean re

