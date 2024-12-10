CC = cc
CFLAGS = -Wall -I/opt/homebrew/include
LDFLAGS = -L/opt/homebrew/lib -lSDL2
TARGET = build/program 
SRCS = src/main.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS) $(LDFLAGS)

clean:
	rm -f $(TARGET)
