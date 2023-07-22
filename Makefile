# # **************************************************************************** #
# #                                                                              #
# #                                                         :::      ::::::::    #
# #    Makefile                                           :+:      :+:    :+:    #
# #                                                     +:+ +:+         +:+      #
# #    By: adbaich <adbaich@student.42.fr>            +#+  +:+       +#+         #
# #                                                 +#+#+#+#+#+   +#+            #
# #    Created: 2022/09/27 15:59:12 by adbaich           #+#    #+#              #
# #    Updated: 2023/07/10 18:41:11 by adbaich          ###   ########.fr        #
# #                                                                              #
# # **************************************************************************** #

SRC= request/parse_request.cpp \
	networking/Init.cpp \
	networking/server.cpp \
	config_file_parsing/parse_config.cpp\
	response/DeleteMethod.cpp\
	response/GetMethod.cpp\
	response/MandatoryResponseHeaders.cpp\
	response/PostMethod.cpp\
	response/utils-map.cpp\
	response/utils.cpp\
	response/CGI.cpp\
	main.cpp\
	Client.cpp\
	

OBJDIR = o

SRC_FILES := $(wildcard $(SRC)/%.cpp)

OBJ := $(patsubst %.cpp,$(OBJDIR)/%.o, ${SRC})

CC= c++

#-Wall -Wextra -Werror

CPPFLAGS= -std=c++98 -Wall -Wextra -Werror

NAME = webserv

all: ${OBJDIR} ${NAME}

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: %.cpp
	@${CC} ${CPPFLAGS} -c $< -o $@

${NAME}: ${OBJ}
	@${CC} ${CPPFLAGS} ${OBJ} -o ${NAME}

clean:
	@rm -f ${OBJ}
fclean: clean
	@rm -f ${NAME}
re: fclean all
