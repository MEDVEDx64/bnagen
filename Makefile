SRCDIR=src
OBJS=$(SRCDIR)/bnagen_main.o $(SRCDIR)/drawthemap.o $(SRCDIR)/loader.o $(SRCDIR)/params.o

bin_path=/usr/bin
share_path=/usr/share/bnagen

CC=gcc
LIBS=-lSDL -lSDLmain -lSDL_image -lSDL_ttf -lSDL_gfx
CFLAGS=-s -O2 -Wall -DSHARE_DIR='"$(share_path)/"'

executable=bnagen
openwith=bnagen-openwith.sh

bnagen: $(OBJS)
	$(CC) -o $(executable) $(CFLAGS) $(LIBS) $(OBJS)

clean:
	rm $(SRCDIR)/*.o

install:
	cp $(executable) $(bin_path)/
	cp $(openwith) $(bin_path)/
	mkdir $(share_path)
	cp font.ttf $(share_path)/

uninstall:
	rm $(bin_path)/$(executable)
	rm $(bin_path)/$(openwith)
	rm -r $(share_path)
