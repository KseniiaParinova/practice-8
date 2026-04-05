CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude
LDFLAGS = 

SRCS = src/main.c src/http_request.c src/http_response.c
OBJS = $(SRCS:.c=.o)
TARGET = httplib

.PHONY: all clean install test

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

install: $(TARGET)
	cp $(TARGET) /usr/local/bin/

test: $(TARGET)
	./$(TARGET)

# Для использования как статической библиотеки
lib: $(OBJS)
	ar rcs libhttp.a $(filter-out src/main.o, $(OBJS))

# Для использования как динамической библиотеки
shared: CFLAGS += -fPIC
shared: $(filter-out src/main.o, $(SRCS:.c=.o))
	$(CC) -shared -o libhttp.so $^