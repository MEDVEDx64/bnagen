SRCDIR=src
SDL_SAVEIMAGE_DIR=$(SRCDIR)/SDL_imagesave
OBJS=$(SRCDIR)/bnagen_main.o $(SRCDIR)/drawthemap.o $(SRCDIR)/loader.o $(SRCDIR)/params.o \
	$(SRCDIR)/convert.o $(SRCDIR)/pixel.o $(SDL_SAVEIMAGE_DIR)/IMG_savepng.o

bin_path=/usr/bin
share_path=/usr/share/bnagen

CC=gcc
LIBS=-lSDL -lSDLmain -lSDL_image -lSDL_ttf -lSDL_gfx -lpng

INCLUDEDIR=-I/usr/include/SDL/ -I./$(SDL_SAVEIMAGE_DIR)/
CFLAGS=-s -O2 -Wall -DSHARE_DIR='"$(share_path)/"' $(INCLUDEDIR)

executable=bnagen
openwith=bnagen-openwith.sh

bnagen: $(OBJS)
	$(CC) -o $(executable) $(CFLAGS) $(LIBS) $(OBJS)

clean:
	rm $(SRCDIR)/*.o
	rm $(SDL_SAVEIMAGE_DIR)/*.o

install:
	cp $(executable) $(bin_path)/
	cp $(openwith) $(bin_path)/
	mkdir $(share_path)
	cp font.ttf $(share_path)/

uninstall:
	rm $(bin_path)/$(executable)
	rm $(bin_path)/$(openwith)
	rm -r $(share_path)
