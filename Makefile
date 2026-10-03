CC := gcc
CFLAGS := -Wall -pedantic-errors -lncursesw -lm
SOURCES := $(wildcard *.c)
OBJECTS := $(SOURCES:.c=.o)
TARGET = cursedpong
INCLUDES = 

all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(INCLUDES)

clean:
	rm -f $(TARGET) $(OBJECTS)
