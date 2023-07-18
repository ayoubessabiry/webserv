CC= c++
SRC= request/parse_request.cpp \
	networking/Init.cpp \
	networking/server.cpp \
	config_file_parsing/parse_config.cpp\
	src/DeleteMethod.cpp\
	src/GetMethod.cpp\
	src/MandatoryResponseHeaders.cpp\
	src/PostMethod.cpp\
	src/utils-map.cpp\
	src/utils.cpp\
	src/CGI.cpp\
	main.cpp\
	client.cpp\

CPPFLAGS= -Wall -Wextra -Werror -std=c++98
NAME = webserv
all : $(NAME)

$(NAME) : $(SRC)
	$(CC) $(CPPFLAG) $(SRC) -o $(NAME)

clean : 
	rm -f $(NAME)

fclean : clean 

re : fclean all