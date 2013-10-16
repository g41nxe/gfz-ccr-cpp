CPPFLAGS=-g -O3

subdirs:
	cd src; make
	mv *.o bin/

all: subdirs
	cd bin; g++ $(CPPFLAGS) *.o -o run	

clean:
	rm -f *.o
	cd bin; rm -f *.o *.exe *.stackdump