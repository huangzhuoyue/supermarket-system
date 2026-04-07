CC = gcc
CFLAGS = -Wall -Isrc/core -Isrc/api -Isrc/utils -Isrc/cli -Isrc/web -g
LDFLAGS = -lws2_32

CORE_SRCS = $(wildcard src/core/*.c)
API_SRCS = $(wildcard src/api/*.c)
UTILS_SRCS = $(wildcard src/utils/*.c)
CLI_SRCS = $(wildcard src/cli/*.c)
WEB_SRCS = $(wildcard src/web/*.c)

SRCS = $(CORE_SRCS) $(API_SRCS) $(UTILS_SRCS) $(CLI_SRCS) $(WEB_SRCS)
OBJS = $(SRCS:.c=.o)

TARGET = supermarket

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
