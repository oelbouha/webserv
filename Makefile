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

INC := -I. \
	-I./include \
	-I./include/interfaces \
	-I./include/exceptions

TMP := -I/goinfre/ysalmi/brew/opt/llvm/include

LINKER := -L/goinfre/ysalmi/brew/opt/llvm/lib/c++ \
		-Wl,-rpath,/goinfre/ysalmi/brew/opt/llvm/lib/c++

TEST_INC := -I/goinfre/ysalmi/brew/Cellar/googletest/1.14.0/include

TEST_LIB := -lgtest -L/goinfre/ysalmi/brew/Cellar/googletest/1.14.0/lib

# TEST_FLAGS := $(TEST_INC) $(TEST_LIB)

CPPFLAGS := -Wall -Wextra -Werror $(INC) $(TMP)


BASE := Config.cpp \
	ConfigHelper.cpp \
	ConfigParser.cpp \
	ConfigParserFactory.cpp \
	Utils.cpp \
	exceptions/ParserException.cpp

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
	@$(CC) $(CPPFLAGS) $(TEST_INC) $(TEST_LIB) -o test $(BASE_OBJ) tests/test$(TEST_ARGS).cpp
	@./test
	@rm test

$(NAME): $(OBJ) #obj/main.o obj/Config.o obj/ConfigHelper.o obj/ConfigParser.o obj/ConfigParserFactory.o
	$(CC) $(CPPFLAGS) -o $@ $^


obj/%.o: src/%.cpp
	@mkdir -p $(shell dirname $@)
	$(CC) $(CPPFLAGS) -o $@ -c $<

clean:
	rm -rf obj/

fclean:
	rm -rf obj/ $(NAME)

re: fclean all

.PHONY: all clean fclean re
