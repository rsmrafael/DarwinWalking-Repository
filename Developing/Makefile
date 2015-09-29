
TARGET = marcha

INCLUDE_DIRS = -I../../Framework/include -I../../Linux/include -I.

CXX = g++
CXXFLAGS += -g -O2 -DLINUX -Wall $(INCLUDE_DIRS)
#CXXFLAGS += -O2 -DDEBUG -DLINUX -Wall $(INCLUDE_DIRS)
LFLAGS += -g -lpthread -ljpeg -lrt

OBJECTS = TauraWalking.o AttitudeEstimation.o MotionPattern.o Omniwalk.o util.o main.o

all: $(TARGET)

clean:
	rm -f *.a *.o $(TARGET) core *~ *.so *.lo

libclean:
	make -C ../../Linux/build clean

distclean: clean libclean

../../Linux/lib/darwin.a:
	make -C ../../Linux/build

$(TARGET): ../../Linux/lib/darwin.a $(OBJECTS)
	$(CXX) $(CFLAGS) $(OBJECTS) ../../Linux/lib/darwin.a -o $(TARGET) $(LFLAGS) 
	chmod 755 $(TARGET)

