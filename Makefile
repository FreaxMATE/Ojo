CC = gcc
GTKVLCFLAGS = `pkg-config --libs --cflags gtk+-3.0 libvlc`
XFLAGS = -lX11
CFLAGS = -rdynamic -Wall

ojo: main.o ojo-player.o ojo-window.o ojo-playlist.o

	$(CC) -o ojo main.o ojo-player.o ojo-window.o ojo-playlist.o $(GTKVLCFLAGS) $(CFLAGS) $(XFLAGS)

main.o: src/main.c

	$(CC) -c src/main.c $(GTKVLCFLAGS) $(CFLAGS) $(XFLAGS)

ojo-player.o: src/ojo-player.c src/ojo-player.h

	$(CC) -c src/ojo-player.c $(GTKVLCFLAGS) $(CFLAGS) $(XFLAGS)

ojo-window.o: src/ojo-window.c src/ojo-window.h

	$(CC) -c src/ojo-window.c $(GTKVLCFLAGS) $(CFLAGS) $(XFLAGS)

ojo-playlist.o: src/ojo-playlist.c src/ojo-playlist.h

	$(CC) -c src/ojo-playlist.c $(GTKVLCFLAGS) $(CFLAGS) $(XFLAGS)

clean:

	rm -f ojo
	rm -f *.o
	rm -f glade/window_main.glade~

install: ojo

	sudo cp ojo /usr/local/bin/ojo
	sudo cp Ojo.desktop /usr/local/share/applications/Ojo.desktop
	sudo cp data/ojoIcon.svg /usr/local/share/icons/ojoIcon.svg
	sudo cp data/ojoIconAboutDialog.png /usr/local/share/icons/ojoIconAboutDialog.png
	
uninstall:

	sudo rm -f /usr/local/bin/ojo
	sudo rm -f /usr/local/share/applications/Ojo.desktop
	sudo rm -f /usr/local/share/icons/ojoIcon.svg
	sudo rm -f /usr/local/share/icons/ojoIconAboutDialog.png
