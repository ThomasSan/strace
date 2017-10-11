NAME = strace

SRC = main.c

INCLUDE = -I includes/

FLAG = -Wall -Werror -Wextra -g

LIBRARIES =

CFILES = $(addprefix src/, $(SRC))

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME):
	@gcc -c $(FLAG) $(INCLUDE) $(CFILES)
	@gcc -o $(NAME) $(LIBRARIES) $(OBJ) $(OBJ2) $(OBJ3) $(OBJ4)
	@mkdir obj && mv $(OBJ) obj
	@echo "\x1b[32;02m$(NAME) ready\x1b[0;m"

clean:
	@/bin/rm -rf obj
	@/bin/rm -rf $(OBJ)
	@echo "\x1b[0;01mclean\x1b[0;m"

fclean: clean
	@/bin/rm -f $(NAME)
	@echo "\x1b[0;01mfclean\x1b[0;m"

re: fclean all