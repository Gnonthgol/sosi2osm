PROGNAME=sosi2osm
OBJFILES=sosi2osm.o sosi.o tag.o node.o

CPPFLAGS = -I /usr/local/include/fyba/ `pkg-config --cflags lua5.1-c++` -DLINUX -DUNIX -g
LDFLAGS = -lfyba -lfygm -lfyut -lproj `pkg-config --libs lua5.1-c++`

all: $(PROGNAME)

clean:
	$(RM) $(PROGNAME) $(OBJFILES) *~

$(PROGNAME): $(OBJFILES)
	$(CXX) $^ $(LDFLAGS) -o $@
