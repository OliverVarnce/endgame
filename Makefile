NAME = 	endgame

SRCS = 	src/*.c		

INC = 	inc/header.h

LIBS =  `src/sdl/bin/sdl2-config --libs`

CFLAGS = -std=c11 -Wall -Wextra -Werror -Wpedantic 
all: $(NAME)

$(NAME): install clean

install:
	@cp $(SRCS) .
	@cp $(INC) .
	@clang $(CFLAGS) -o $(NAME) $(SRCS) -O3 `src/sdl/bin/sdl2-config --cflags` $(LIBS)


uninstall: clean
	@rm -rf $(NAME)

clean:
	@rm -rf *.h
	@rm -rf *.c \

reinstall: all 
