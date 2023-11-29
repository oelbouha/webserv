#|	                     __          _
#|	   __  ___________ _/ /___ ___  (_)
#|	  / / / / ___/ __ `/ / __ `__ \/ /
#|	 / /_/ (__  ) /_/ / / / / / / / / 
#|	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
#|	/____/	User: Youssef Salmi
#|			File: Makefile 
#|

NAME := webserv

##########################################################################
#	Compiler Variables

CXX := clang++

INC := -I. \
	-I./include \
	-I./include/interfaces \
	-I./include/exceptions

TMP := -I/goinfre/ysalmi/brew/opt/llvm/include

CPPFLAGS := -Wall -Wextra -Werror $(INC) $(TMP)

LINKER := -L/goinfre/ysalmi/brew/opt/llvm/lib/c++ \
		-Wl,-rpath,/goinfre/ysalmi/brew/opt/llvm/lib/c++

TEST_INC := -I/goinfre/ysalmi/brew/Cellar/googletest/1.14.0/include

TEST_LIB := -lgtest -L/goinfre/ysalmi/brew/Cellar/googletest/1.14.0/lib

TEST_FLAGS := $(TEST_INC) $(TEST_LIB)


##########################################################################
#	Source and Object files

# BASE := WebServer.cpp \
# 	Config.cpp \
# 	ConfigHelper.cpp \
# 	ConfigParser.cpp \
# 	ConfigParserFactory.cpp \
# 	Socket.cpp \
# 	Utils.cpp \
# 	exceptions/ParserException.cpp \
# 	exceptions/SocketException.cpp

BASE := WebServer.cpp \
	Socket.cpp \
	Utils.cpp

CONFIG_PARSER_COMPONENT := ConfigParser/Config.cpp \
	ConfigParser/ConfigHelper.cpp \
	ConfigParser/ConfigParser.cpp \
	ConfigParser/ConfigParserFactory.cpp \
	ConfigParser/ParserException.cpp

SOCKET_COMPONENT := Socket/ClientSocket.cpp \
	Socket/ServerSocket.cpp \
	Socket/SocketException.cpp

REQUEST_COMPONENT := Request/Request.cpp \
	Request/RequestException.cpp
#	Request/RequestParser.cpp

RESPONSE_COMPONENT := Response/Response.cpp \
	Response/ResponseException.cpp 

SRC := main.cpp \
	$(BASE) \
	$(CONFIG_PARSER_COMPONENT) \
	$(SOCKET_COMPONENT) \
	$(REQUEST_COMPONENT) \
	$(RESPONSE_COMPONENT)

BASE_OBJ := $(addprefix obj/,$(BASE:.cpp=.o))
OBJ := $(addprefix obj/,$(SRC:.cpp=.o))


##########################################################################
#	Args Extractions

define template
.PHONY: $(1)
$(1):
	@:
endef

ifeq (test,$(firstword $(MAKECMDGOALS)))
  TEST_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  $(foreach arg,$(TEST_ARGS),$(eval $(call template,$(arg))))
endif

##########################################################################
#	Rules

all: $(NAME)


test: $(BASE_OBJ) tests/test$(TEST_ARGS).cpp tests/test.h
	@$(CXX) $(CPPFLAGS) $(TEST_FLAGS) -o test $(BASE_OBJ) tests/test$(TEST_ARGS).cpp
	@./test
	@rm test



$(NAME): $(OBJ)
	$(CXX) $(CPPFLAGS) -o $@ $^



obj/%.o: src/%.cpp
	@mkdir -p $(shell dirname $@)
	$(CXX) $(CPPFLAGS) -o $@ -c $<



clean:
	rm -rf obj/

fclean:
	rm -rf obj/ $(NAME)

re: fclean all

.PHONY: all clean fclean re
