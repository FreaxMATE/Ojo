CC = gcc
GTKVLCFLAGS = `pkg-config --libs --cflags gtk+-3.0 libvlc`
XFLAGS = -lX11
CFLAGS = -rdynamic -Wall

ojo: main.o window.o vlcPlayer.o

	$(CC) -o ojo main.o window.o vlcPlayer.o $(GTKVLCFLAGS) $(CFLAGS) $(XFLAGS)

main.o: src/main.c

	$(CC) -c src/main.c $(GTKVLCFLAGS) $(CFLAGS) $(XFLAGS)

window.o: src/window.c src/window.h

	$(CC) -c src/window.c $(GTKVLCFLAGS) $(CFLAGS) $(XFLAGS)

vlcPlayer.o: src/vlcPlayer.c src/vlcPlayer.h

	$(CC) -c src/vlcPlayer.c $(GTKVLCFLAGS) $(CFLAGS) $(XFLAGS)

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
