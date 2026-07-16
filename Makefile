CPP = c++

NAME = irc

FLAGS = -Wall -Wextra -Werror -std=c++98 

RM = rm -rf

SRC = srcs/main.cpp srcs/server.cpp srcs/client.cpp

HEADER = includes/server.hpp

OBJ = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CPP) $(OBJ) -o $(NAME)

%.o: %.cpp $(HEADER)
	$(CPP) $(FLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re : fclean all

.PHONY : clean