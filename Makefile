PROGNAME=sosi2osm
OBJFILES=sosi2osm.o sosi.o tag.o node.o

CPPFLAGS := $(CPPFLAGS) `pkg-config --cflags lua5.1-c++ fyba` -DLINUX -DUNIX -g
LDFLAGS := $(LDFLAGS) -lproj `pkg-config --libs lua5.1-c++ fyba`

all: $(PROGNAME)

clean:
	$(RM) $(PROGNAME) $(OBJFILES) *~

$(PROGNAME): $(OBJFILES)
	$(CXX) $^ $(LDFLAGS) -o $@

install: sosi2osm
	install -m 0755 sosi2osm /usr/local/bin
	mkdir -p /usr/local/share/sosi2osm/lua
	install -m 0644 lua/*.lua /usr/local/share/sosi2osm/lua

uninstall:
	rm /usr/local/share/sosi2osm/lua/*.lua
	rmdir /usr/local/share/sosi2osm/lua
	rmdir /usr/local/share/sosi2osm
	rm /usr/local/bin/sosi2osm

.PHONY: install uninstall clean all
