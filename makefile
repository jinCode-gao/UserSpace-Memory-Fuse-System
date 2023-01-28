CC = gcc
CFLAGS = -Wall -g -I/usr/local/include/fuse3
LDFLAGS = -lfuse3
TARGET = hello

all: $(TARGET)

$(TARGET): hello.c
	$(CC) $(CFLAGS) -o $(TARGET) hello.c $(LDFLAGS)

clean:
	rm -f $(TARGET)
