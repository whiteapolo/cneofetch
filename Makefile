CC=gcc
CFLAGS = -Wall -o2

SRC = $(wildcard src/*.c) $(wildcard src/utils/*.c)

all: $(SRC)
	$(CC) $^ -o exe

cneofetch: $(SRC)
	$(CC) -o2 $^ -o $@

install: $(SRC)
	$(CC) -o2 $^ -o cneofetch
	mv cneofetch /usr/local/bin 

uninstall:
	rm -rf /usr/local/bin/cneofetch

clean:
	$(RM) -r exe cneofetch
