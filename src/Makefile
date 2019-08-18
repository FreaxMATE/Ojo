CC = gcc
GTKVLCFLAGS = `pkg-config --libs gtk+-3.0 libvlc` `pkg-config --cflags gtk+-3.0 libvlc`
XFLAGS = -lX11
CFLAGS = -Wall

player: player.c

	$(CC) -o player player.c $(GTKVLCFLAGS) $(CFLAGS) $(XFLAGS)

clean:

	rm player
