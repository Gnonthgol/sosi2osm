PROGNAME=sosi2osm
OBJFILES=sosi2osm.o sosi.o

CPPFLAGS = -I /usr/local/include/fyba/ -DLINUX -DUNIX -g
LDFLAGS = -lfyba -lfygm -lfyut -lproj

all: $(PROGNAME)

clean:
	$(RM) $(PROGNAME) $(OBJFILES) *~

$(PROGNAME): $(OBJFILES)
	$(CXX) $^ $(LDFLAGS) -o $@
