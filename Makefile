CC = gcc
CFLAGS = -Wall -Isrc/core -Isrc/api -Isrc/utils -Isrc/cli -g

CORE_SRCS = $(wildcard src/core/*.c)
API_SRCS = $(wildcard src/api/*.c)
UTILS_SRCS = $(wildcard src/utils/*.c)
CLI_SRCS = $(wildcard src/cli/*.c)

SRCS = $(CORE_SRCS) $(API_SRCS) $(UTILS_SRCS) $(CLI_SRCS)
OBJS = $(SRCS:.c=.o)

TARGET = supermarket

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
