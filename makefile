CC = gcc
CFLAGS = -Wall -Wextra -std=c11
SRCS = main.c http.c
OBJS = $(SRCS:.c=.o)
TARGET = logger

all: $(TARGET)

$(TARGET): $(OBJS)
    $(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.c
    $(CC) $(CFLAGS) -c $< -o $@

clean:
    rm -f $(OBJS) $(TARGET)