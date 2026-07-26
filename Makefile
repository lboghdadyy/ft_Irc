CXX = c++

NAME = ircserv

CXXFLAGS = -Wall -Wextra -Werror -std=c++98 

RM = rm -rf

SRC = main.cpp srcs/server.cpp srcs/client.cpp srcs/channels.cpp \
	srcs/commands/Commands.cpp srcs/commands/join.cpp srcs/commands/kick.cpp srcs/commands/invite.cpp srcs/commands/pass.cpp srcs/commands/user.cpp srcs/commands/nick.cpp

HEADER = includes/server.hpp includes/channels.cpp includes/client.cpp

OBJ = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(OBJ) -o $(NAME)

%.o: %.cpp $(HEADER)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re : fclean all

.PHONY : clean
