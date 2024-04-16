CC = gcc
CFLAGS = -Wall -o3

all:
	$(CC)  $(CFLAGS) main.c -o exe
install:
	$(CC)  $(CFLAGS) main.c -o cneofetch
	mv cneofetch /usr/local/bin
clean:
	rm -rf exe cneofetch
uninstall:
	rm -rf /usr/local/bin/cneofetch

.PHONY: all clean
