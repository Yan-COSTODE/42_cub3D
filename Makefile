CC=cc
NAME = cub3D
SRCPATH = srcs/
LIBMLX	= MLX42
SRCS = gnl.c libft.c main.c map.c minimap.c move.c parsing.c print.c raycast.c shoot.c split.c
SRCSINC = $(addprefix $(SRCPATH), $(SRCS))
HEADERS = -I ./include -I $(LIBMLX)/include
OBJS = $(SRCSINC:.c=.o)
CFLAGS = -Wall -Wextra -Werror -g
LFLAGS = $(LIBMLX)/build/libmlx42.a -ldl -lglfw -pthread -lm

all: $(NAME)

%.o : %.c
	@echo -n "\033[1;37m"
	$(CC) $(CFLAGS) -o $@ -c $< $(HEADERS)

$(NAME): $(OBJS)
	@echo "\033[1;32mCompiling MLX42 !\033[0m"
	@cmake $(LIBMLX) -B $(LIBMLX)/build && make --no-print-directory -C $(LIBMLX)/build -j4
	@echo "\033[1;32mMLX42 Finish Compilation !\033[0m"
	@echo -n "\033[1;37m"
	$(CC) $(OBJS) $(LFLAGS) $(HEADERS) -o $(NAME)
	@echo "\033[1;32mCompilation Finished !\033[0m"

fclean: clean
	@echo -n "\033[1;37m"
	rm -f $(NAME)
	rm -rf $(LIBMLX)/build
	@echo "\033[1;31mProgram Deleted !\033[0m"

clean:
	@printf "\033[1;37m"
	rm -f $(OBJS)
	@echo "\033[1;31mObjects Deleted !\033[0m"

re: fclean all

.PHONY: all fclean clean re
