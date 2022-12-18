BASENAME = dharma
PROGRAMNAME = Dharma
VERSION = "0.1.0"

SDIR = src

IDIR = include
LOCALENAME = $(BASENAME)

CCCMD = gcc
CFLAGS = -I$(IDIR) `pkg-config --cflags --libs gtk+-3.0` -Wall -Wextra -Wpedantic -Wno-deprecated-declarations -DLOCALE_=\"$(LOCALENAME)\" -pthread -DPROGRAMNAME=\"$(PROGRAMNAME)\"

debug: CC = $(CCCMD) -DDEBUG_ALL -DVERSION=\"$(VERSION)_DEBUG\"
debug: BDIR = build

release: CC = $(CCCMD) -DVERSION=\"$(VERSION)\" -O2
release: BDIR = build

windows: CC = $(CCCMD) -DVERSION=\"$(VERSION)\" -O2 -mwindows
windows: BDIR = build
windows_GTKENV: BDIR = build

install: CC = $(CCCMD) -DMAKE_INSTALL -DVERSION=\"$(VERSION)\"
install: PROGDIR = /usr/lib/$(BASENAME)
install: BDIR = $(PROGDIR)/bin

archlinux: CC = $(CCCMD) -DMAKE_INSTALL -DVERSION=\"$(VERSION)\"

ODIR=.obj/linux
WODIR=.obj/win
DODIR=.obj/debug
LDIR=lib

LIBS = -lm -lpthread

_DEPS = dharma_defines.h dharma_image.h dharma_color.h dharma_math.h dharma_session.h gui_templates.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = dharma.o dharma_image.o dharma_color.o dharma_math.o dharma_session.o gui_templates.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))
WOBJ = $(patsubst %,$(WODIR)/%,$(_OBJ))
DOBJ = $(patsubst %,$(DODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	mkdir -p $(ODIR)
	$(CC) -c -o $@ $< $(CFLAGS)

$(DODIR)/%.o: $(SDIR)/%.c $(DEPS)
	mkdir -p $(DODIR)
	$(CC) -c -o $@ $< $(CFLAGS)

release: $(OBJ)
	mkdir -p $(BDIR)
	mkdir -p $(ODIR)
	$(CC) -o $(BDIR)/$(BASENAME) $^ $(CFLAGS) $(LIBS)

debug: $(DOBJ)
	mkdir -p $(BDIR)
	mkdir -p $(DODIR)
	$(CC) -o $(BDIR)/$(BASENAME)_DEBUG $^ $(CFLAGS) $(LIBS)

windows: $(WOBJ)
	mkdir -p $(WODIR)
	windres __windows__/my.rc -O coff $(WODIR)/my.res
	windres __windows__/appinfo.rc -O coff $(WODIR)/appinfo.res
	mkdir -p $(BDIR)
	$(CC) -o $(BDIR)/$(BASENAME) $^ $(CFLAGS) $(LIBS) $(WODIR)/my.res $(WODIR)/appinfo.res

windows_GTKENV: windows
	ldd $(BDIR)/$(BASENAME).exe | grep '\/mingw.*\.dll' -o | xargs -I{} cp "{}" $(BDIR)/
	cp -ru __windows__/windows_assets/* $(BDIR)/
	cp -ru assets $(BDIR)/
	cp -ru locale/ $(BDIR)/
	cp LICENSE $(BDIR)/

install: $(OBJ)
	mkdir -p $(PROGDIR)
	mkdir -p $(BDIR)
	mkdir -p $(ODIR)
	mkdir -p /usr/lib/$(BASENAME)
	$(CC) -o $(BDIR)/$(BASENAME) $^ $(CFLAGS) $(LIBS)
	ln -sf $(BDIR)/$(BASENAME) /usr/bin/$(BASENAME)
	#cp -ru assets/ /usr/lib/$(BASENAME)
	#cp -ru locale/ /usr/lib/$(BASENAME)
	cp LICENSE /usr/lib/$(BASENAME)
	#cp assets/$(BASENAME).desktop /usr/share/applications/
	#cp assets/app_icon/256.png /usr/share/pixmaps/$(BASENAME).png

archlinux: $(OBJ) $(OBJ_GUI)
	mkdir -p $(BDIR)/usr/lib/$(BASENAME)
	mkdir -p $(BDIR)/usr/share/applications
	mkdir -p $(BDIR)/usr/share/pixmaps
	mkdir -p $(BDIR)/usr/bin/
	mkdir -p $(ODIR)
	$(CC) -o $(BDIR)/usr/bin/$(BASENAME) $^ $(CFLAGS) $(LIBS)
	#cp -ru assets/ $(BDIR)/usr/lib/$(BASENAME)/
	#cp -ru locale/ $(BDIR)/usr/lib/$(BASENAME)/
	cp LICENSE $(BDIR)/usr/lib/$(BASENAME)/
	#cp assets/$(BASENAME).desktop $(BDIR)/usr/share/applications/
	#cp assets/app_icon/256.png $(BDIR)/usr/share/pixmaps/$(BASENAME).png

.PHONY: clean
clean:
	rm -f $(ODIR)/*.o $(WODIR)/*.o $(DODIR)/*.o *~ core $(INCDIR)/*~

.PHONY: all
all: release clean
