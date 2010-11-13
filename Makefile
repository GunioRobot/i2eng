#  Programa i2e
#  Fichero Makefile
#  Autor Alfredo Casademunt
#  Fecha 8-1-2000

CC = gcc
#CFLAG = -Wall -g -DG_ENABLE_DEBUG
CFLAG = -Wall -O2
OBJS = i2e.o traduce.o mensaje.o aprende.o funciones.o

i2e :  po/es.mo $(OBJS) Makefile
	$(CC) $(CFLAG) $(OBJS) -o i2e `gtk-config --libs`

i2e.o : i2e.c i2e.h 
	$(CC) $(CFLAG) -c i2e.c -o i2e.o `gtk-config --cflags`

traduce.o : traduce.c i2e.h
	$(CC) $(CFLAG) -c traduce.c -o traduce.o `gtk-config --cflags`

mensaje.o : mensaje.c i2e.h
	$(CC) $(CFLAG) -c mensaje.c -o mensaje.o `gtk-config --cflags`

aprende.o : aprende.c i2e.h
	$(CC) $(CFLAG) -c aprende.c -o aprende.o `gtk-config --cflags`

funciones.o : funciones.c i2e.h
	$(CC) $(CFLAG) -c funciones.c -o funciones.o `gtk-config --cflags`

po/es.mo : po/es.po
	msgfmt -o po/es.mo po/es.po

clean :
	rm -rf *.o *~ *.bak *.mo i2e

install :
	install -g root -o root i2e $(DESTDIR)/usr/X11R6/bin
	install -d $(DESTDIR)/usr/share/locale/es/LC_MESSAGES
	install -g root -o root po/es.mo $(DESTDIR)/usr/share/locale/es/LC_MESSAGES/i2e.mo
	install -g root -o root i2e.sh $(DESTDIR)/usr/bin
	install -g root -o root man/man1/i2e.1.gz $(DESTDIR)/usr/man/man1
	install -d $(DESTDIR)/usr/share/i2e
	install -g root -o root i2e.dict $(DESTDIR)/usr/share/i2e
	install -d $(DESTDIR)/usr/man/es/man1
	install -g root -o root man/es/man1/i2e.1.gz $(DESTDIR)/usr/man/es/man1

uninstall :
	rm $(DESTDIR)/usr/X11R6/bin/i2e
	rm $(DESTDIR)/usr/share/locale/es/LC_MESSAGES/i2e.mo
	rm $(DESTDIR)/usr/bin/i2e.sh
	rm $(DESTDIR)/usr/man/man1/i2e.1.gz
	rm $(DESTDIR)/usr/man/es/man1/i2e.1.gz
	rm $(DESTDIR)/usr/share/i2e/i2e.dict
	rmdir $(DESTDIR)/usr/share/i2e
#	rmdir $(DESTDIR)/usr/man/es/man1
#	rmdir $(DESTDIR)/usr/man/es
#	rmdir $(DESTDIR)/usr/share/locale/es/LC_MESSAGES

