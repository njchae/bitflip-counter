CC = gcc

CFLAGS = -O3 -Wall -W -pedantic -std=c99

TARGET = counter

all: $(TARGET)

$(TARGET): counter.o
	$(CC) $(CFLAGS) -o $(TARGET) counter.o

counter.o: counter.c
	$(CC)  $(CFLAGS) -c counter.c

clean:
	rm -f $(TARGET) *.o counter_data.txt

.PHONY: all clean