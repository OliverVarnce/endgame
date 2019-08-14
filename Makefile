NAME = 	endgame

SRCS = 	src/SDL2_rotozoom.c \
		src/move_mine.c \
		src/mx_printstr.c \
		src/reverse.c \
		src/itoa.c \
		src/mx_printchar.c \
		src/mx_strlen.c \
		src/swap.c \
		src/main.c \
		src/mx_printerr.c \
		src/respawn_trash.c		

INC = 	inc/header.h \
		inc/SDL2_rotozoom.h

SDL =   -F inc/framework -I inc/framework/SDL2.framework/SDL2

CFLAGS = -std=c11 -Wall -Wextra -Werror -Wpedantic\
		 -framework SDL2_ttf \
	 	 -rpath inc/framework -framework SDL2 \
	 	 					  -framework SDL2_image \
	 	 					  -framework SDL2_mixer \
		-I inc/framework/SDL2_image.framework/Headers \
		-I inc/framework/SDL2_ttf.framework/Headers \
		-I inc/framework/SDL2_mixer.framework/Headers

all: $(NAME)

$(NAME): install clean

install:
	@cp $(SRCS) .
	@cp $(INC) .
	@clang $(CFLAGS) -o $(NAME) $(SRCS) $(LIBS) $(SDL) -I inc

uninstall: clean
	@rm -rf $(NAME)

clean:
	@rm -rf header.h \
			SDL2_rotozoom.h
	@rm -rf SDL2_rotozoom.c \
			move_mine.c \
			mx_printstr.c \
			reverse.c \
			itoa.c \
			mx_printchar.c \
			mx_strlen.c \
			swap.c \
			main.c \
			mx_printerr.c \
			respawn_trash.c

reinstall: all 
