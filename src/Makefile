CC = gcc
GTKVLCFLAGS = `pkg-config --libs gtk+-3.0 libvlc` `pkg-config --cflags gtk+-3.0 libvlc`
XFLAGS = -lX11
CFLAGS = -Wall

ojo: main.o menu.o window.o widget.o vlcPlayer.o

	$(CC) -o ojo main.o menu.o window.o widget.o vlcPlayer.o $(GTKVLCFLAGS) $(CFLAGS) $(XFLAGS)

main.o: main.c

	$(CC) -c main.c $(GTKVLCFLAGS) $(CFLAGS) $(XFLAGS)

menu.o: menu.c menu.h

	$(CC) -c menu.c $(GTKVLCFLAGS) $(CFLAGS) $(XFLAGS)

window.o: window.c window.h

	$(CC) -c window.c $(GTKVLCFLAGS) $(CFLAGS) $(XFLAGS)

widget.o: widget.c widget.h

	$(CC) -c widget.c $(GTKVLCFLAGS) $(CFLAGS) $(XFLAGS)

vlcPlayer.o: vlcPlayer.c vlcPlayer.h

	$(CC) -c vlcPlayer.c $(GTKVLCFLAGS) $(CFLAGS) $(XFLAGS)

clean:

	rm -f ojo && rm -f *.o

install: ojo

	sudo cp ojo /usr/local/bin/ojo
	sudo cp Ojo.desktop /usr/local/share/applications/Ojo.desktop
	sudo cp ../data/ojoIcon.svg /usr/local/share/icons/ojoIcon.svg
	
uninstall:

	sudo rm -f /usr/local/bin/ojo
	sudo rm -f /usr/local/share/applications/ojo.desktop
	sudo rm -f /usr/local/share/icons/ojoIcon.svg
	
	


