NAME = ircserv

CXX = c++

CPPFLAGS = -Wall -Wextra -Werror -std=c++98

SRC = main.cpp commands/Invite.cpp commands/Jion.cpp commands/Kick.cpp

INC = Server.hpp Channel.hpp Client.hpp replies.hpp 

RM = rm -f

OBJ = $(SRC:.cpp=.o)

all : $(NAME)

$(NAME) : $(OBJ)
	$(CXX) $(CPPFLAGS) $(OBJ) -o $(NAME)

%.o : %.cpp $(INC)
	$(CXX) $(CPPFLAGS) -c $< -o $@

clean :
	$(RM) $(OBJ)

fclean : clean
	$(RM) $(NAME)

re : fclean all

.PHONY : clean