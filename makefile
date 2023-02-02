CC = gcc
CFLAGS = -Wall -g -I/usr/local/include/fuse3
LDFLAGS = -lfuse3
TARGET = UMFS

all: $(TARGET)

$(TARGET): UMFS.c
	$(CC) $(CFLAGS) -o $(TARGET) UMFS.c $(LDFLAGS)

clean:
	rm -f $(TARGET)
