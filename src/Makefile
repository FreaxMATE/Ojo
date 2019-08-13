
CC = gcc
GTKVLCFLAGS = `pkg-config --libs gtk+-2.0 libvlc` `pkg-config --cflags gtk+-2.0 libvlc`
CFLAGS = -Wall

player: player.c

	$(CC) -o player player.c $(GTKVLCFLAGS) $(CFLAGS)

