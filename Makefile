PROGNAME=sosi2osm
OBJFILES=sosi2osm.o sosi.o tag.o node.o

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
# Mac settings
	LUA = lua
	LDFLAGS += -liconv 
else
	LUA = lua5.1-c++
endif

CPPFLAGS := $(CPPFLAGS) `pkg-config --cflags $(LUA) fyba` -DLINUX -DUNIX -g
LDFLAGS := $(LDFLAGS) -lproj `pkg-config --libs $(LUA) fyba` -lfyut -lfygm

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
