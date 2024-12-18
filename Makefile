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

# CXX := clang++
CXX := c++

INC := -I. \
		-I./include \
		-I./src/interfaces

#TMP := -I/goinfre/ysalmi/brew/opt/llvm/include
TMP :=

CPPFLAGS := -Wall -Wextra -Werror $(INC) $(TMP) #-fsanitize=address -g 

LINKER := -L/goinfre/ysalmi/brew/opt/llvm/lib/c++ \
		-Wl,-rpath,/goinfre/ysalmi/brew/opt/llvm/lib/c++

TEST_INC := -I/goinfre/ysalmi/brew/Cellar/googletest/1.14.0/include

TEST_LIB := -lgtest -L/goinfre/ysalmi/brew/Cellar/googletest/1.14.0/lib

TEST_FLAGS := $(TEST_INC) $(TEST_LIB)


##########################################################################
#	Source and Object files

MAIN_COMPONENT := Main/main.cpp \
	Main/WebServer.cpp

UTILS_COMPONENT := Utils.cpp \
	Logger.cpp

DATATYPES_COMPONENT := DataTypes/Config.cpp \
	DataTypes/ConfigException.cpp

CONFIG_PARSER_COMPONENT := ConfigParser/ConfigHelper.cpp \
	ConfigParser/YmlConfigParser.cpp \
	ConfigParser/ConfigParserFactory.cpp \
	ConfigParser/ParserException.cpp

SOCKET_COMPONENT := Socket/ClientSocket.cpp \
	Socket/ServerSocket.cpp \
	Socket/SocketException.cpp

REQUEST_COMPONENT := Request/Request.cpp \
	Request/DefaultReader.cpp \
	Request/ChunkedReader.cpp \
	Request/BufferRequest.cpp \
	Request/RequestException.cpp

RESPONSE_COMPONENT := 	Response/AResponse.cpp \
	Response/FileResponse.cpp \
	Response/BufferResponse.cpp \
	Response/ResponseException.cpp

CGI_COMPONENT	:= CGI/CGIHandler.cpp \
	CGI/ProxyPair.cpp \
	CGI/CGIRequest.cpp \
	CGI/CGIResponse.cpp  \
	CGI/DefaultWriter.cpp \
	CGI/ChunkedWriter.cpp

MULTIPLEXER_COMPONENT := Multiplexer/SelectMultiplexer.cpp \
						 Multiplexer/KqueueMultiplexer.cpp

SERVER_COMPONENT := Server/Result.cpp \
	Server/Cluster.cpp \
	Server/Server.cpp \
	Server/ErrorPages.cpp \
	Server/MimeTypes.cpp \
	Route/Upload.cpp \
	Route/Route.cpp

CLIENT_COMPONENT := Client/Client.cpp

SRC := $(MAIN_COMPONENT) \
	$(DATATYPES_COMPONENT) \
	$(CONFIG_PARSER_COMPONENT) \
	$(SOCKET_COMPONENT) \
	$(REQUEST_COMPONENT) \
	$(RESPONSE_COMPONENT) \
	$(CGI_COMPONENT) \
	$(MULTIPLEXER_COMPONENT) \
	$(SERVER_COMPONENT) \
	$(CLIENT_COMPONENT) \
	$(UTILS_COMPONENT)

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

echo:
	@echo $(SRC)

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
