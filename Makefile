CC = clang
CFLAGS = -Wall -Wextra -pedantic -g -fsanitize=address

TARGET = mygrep
SRCS = main.c grep.c fileio.c 
OBJS = $(SRCS:.c=.o)

.PHONY: clean

$(TARGET): $(OBJS)
		$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)
		
%.o:%.c 
		$(CC) $(CFLAGS) -c $< -o $@

clean:
		rm -f $(OBJS) $(TARGET)

