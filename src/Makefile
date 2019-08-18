CC = gcc
GTKVLCFLAGS = `pkg-config --libs gtk+-3.0 libvlc` `pkg-config --cflags gtk+-3.0 libvlc`
XFLAGS = -lX11
CFLAGS = -Wall

player: main.o vlcPlayer.o

	$(CC) -o player main.o vlcPlayer.o $(GTKVLCFLAGS) $(CFLAGS) $(XFLAGS)

main.o: main.c

	$(CC) -c main.c $(GTKVLCFLAGS) $(CFLAGS) $(XFLAGS)

vlcPlayer.o: vlcPlayer.c vlcPlayer.h

	$(CC) -c vlcPlayer.c $(GTKVLCFLAGS) $(CFLAGS) $(XFLAGS)

clean:

	rm player && rm *.o
