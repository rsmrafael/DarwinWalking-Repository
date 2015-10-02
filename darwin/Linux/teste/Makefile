###############################################################
#
# Purpose: Makefile for "teste"
# Author.: TauraBots
# Version: 0.1
# License: GPL
#
###############################################################

TARGET = teste

INCLUDE_DIRS = -I../include -I../../Framework/include

CXX = g++
CXXFLAGS += -g -O2 -DLINUX -Wall $(INCLUDE_DIRS)
#CXXFLAGS += -O2 -DDEBUG -DLINUX -Wall $(INCLUDE_DIRS)
LFLAGS += -g -lpthread -ljpeg -lrt

OBJECTS = teste.o

all: $(TARGET)

clean:
	rm -f *.a *.o $(TARGET) core *~ *.so *.lo

libclean:
	make -C ../build clean

distclean: clean libclean

darwin.a:
	make -C ../build

$(TARGET): darwin.a $(OBJECTS)
	$(CXX) $(CFLAGS) $(OBJECTS) ../lib/darwin.a -o $(TARGET) $(LFLAGS)
	chmod 755 $(TARGET)

