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
	rm -rf *.o *~ *.bak i2e
	rm -f man/index.bt
	rm -f po/*.mo

install :
	install -m 755 -g root -o root i2e $(DESTDIR)/usr/bin
	install -d $(DESTDIR)/usr/share/locale/es/LC_MESSAGES
	install -m 644 -g root -o root po/es.mo $(DESTDIR)/usr/share/locale/es/LC_MESSAGES/i2e.mo
	install -m 755 -g root -o root i2e.sh $(DESTDIR)/usr/bin/i2e-cli
	install -d $(DESTDIR)/usr/share/man/man1
	install -m 644 -g root -o root man/man1/i2e.1x $(DESTDIR)/usr/share/man/man1
	install -m 644 -g root -o root man/man1/i2e-cli.1 $(DESTDIR)/usr/share/man/man1
	install -d $(DESTDIR)/usr/share/i2e
	install -m 644 -g root -o root i2e.dict $(DESTDIR)/usr/share/i2e
	install -d $(DESTDIR)/usr/share/man/es/man1
	install -m 644 -g root -o root man/es/man1/i2e.es.1 $(DESTDIR)/usr/share/man/es/man1/i2e.1

uninstall :
	rm $(DESTDIR)/usr/bin/i2e
	rm $(DESTDIR)/usr/share/locale/es/LC_MESSAGES/i2e.mo
	rm $(DESTDIR)/usr/bin/i2e-cli
	rm $(DESTDIR)/usr/share/man/man1/i2e.1
	rm $(DESTDIR)/usr/share/man/es/man1/i2e.1
	rm $(DESTDIR)/usr/share/i2e/i2e.dict
	rmdir $(DESTDIR)/usr/share/i2e
#	rmdir $(DESTDIR)/usr/man/es/man1
#	rmdir $(DESTDIR)/usr/man/es
#	rmdir $(DESTDIR)/usr/share/locale/es/LC_MESSAGES

