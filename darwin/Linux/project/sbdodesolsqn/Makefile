###############################################################
#
# Purpose: Makefile for "demo"
# Author.: robotis
# Version: 0.1
# License: GPL
#
###############################################################

TARGET = demo

INCLUDE_DIRS = -I../../include -I../../../Framework/include

CXX = g++
CXXFLAGS += -O2 -DLINUX -Wall $(INCLUDE_DIRS)
LFLAGS += -lpthread -ljpeg -lrt

OBJECTS = TauraTracker.o main.o

all: $(TARGET)

clean:
	rm -f *.a *.o $(TARGET) core *~ *.so *.lo

libclean:
	make -C ../../build clean

distclean: clean libclean

../../lin/darwin.a:
	make -C ../../build

$(TARGET): ../../lin/darwin.a $(OBJECTS)
	$(CXX) $(CFLAGS) $(OBJECTS) ../../lib/darwin.a -o $(TARGET) $(LFLAGS)
	chmod 755 $(TARGET)

# useful to make a backup "make tgz"
tgz: clean
	mkdir -p backups
	tar czvf ./backups/DARwIn_demo_`date +"%Y_%m_%d_%H.%M.%S"`.tgz --exclude backups *
