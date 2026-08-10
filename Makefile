CXX = c++

GREEN:=\033[0;32m
YELLOW := \033[0;33m

NAME = ircserv

CXXFLAGS = -Wall -Wextra -Werror -std=c++98

RM = rm -rf

SRC = main.cpp srcs/server.cpp srcs/client.cpp srcs/channels.cpp \
	srcs/commands/Commands.cpp srcs/commands/join.cpp srcs/commands/kick.cpp srcs/commands/invite.cpp srcs/commands/pass.cpp srcs/commands/user.cpp srcs/commands/nick.cpp \
	srcs/commands/privmsg.cpp srcs/commands/quit.cpp srcs/commands/topic.cpp srcs/commands/mode.cpp srcs/channels_utils.cpp srcs/server_utils.cpp

HEADER = includes/server.hpp includes/channels.hpp includes/client.hpp

OBJ = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	@$ $(CXX) $(OBJ) -fsanitize=address -o $(NAME)
	@echo "$(GREEN)IRC serever built successfully ."
	

%.o: %.cpp $(HEADER)
	@$ $(CXX) $(CXXFLAGS) -c $< -o $@
	
clean:
	@$ $(RM) $(OBJ)
	@echo "$(YELLOW)object files were removed."

fclean: clean
	@$ $(RM) $(NAME)
	@echo "$(YELLOW)IRC program and object files were removed."

re : fclean all

.PHONY : clean
