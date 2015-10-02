###############################################################
#
# Purpose: Makefile for the line detection demo
# Author.: Taura Bots
# Version: 0.1
# License: GPL
#
###############################################################

TARGET1 := vis_streaming
TARGET2 := vis_image
TARGET3 := capturecolor
TARGET4 := showcolor
TARGET5 := capturelimits
TARGET6 := captureball
TARGET7 := capturepoles

INCLUDE_DIRS = -I../../include -I../../../Framework/include -I./include

C = gcc
CFLAGS += -O2 -Wall -fPIC $(INCLUDE_DIRS)
CXX = g++
CXXFLAGS += -O2 -DLINUX -Wall $(INCLUDE_DIRS)
LFLAGS += -lpthread -ljpeg -lrt -lSDL

OBJECTS := FeaturePointFinder.o VisionUtil.o FeaturePoint.o jpeg_util.o cblob.o BlobScan.o SDLWindow.o FieldLimitsFinder.o BallFinder.o LinesFinder.o

OBJECTS := $(addprefix obj/, $(OBJECTS))

NUMBERS1 := 0 1 2 3 4 5 6 7 8 9 10 11 12 13
TESTS := $(foreach var,$(NUMBERS1),case$(var).jpg )

NUMBERS2 := 1 2 3 4 5 6 7 8
TESTS += $(foreach var,$(NUMBERS2),pole$(var).jpg )

TESTS += fieldcrossings.jpg fieldlines.jpg

TESTS := $(addprefix test_results/, $(TESTS))

.SECONDARY: $(foreach var,$(NUMBERS1),test_results/case$(var)_grass.color) $(foreach var,$(NUMBERS2),test_results/pole$(var)_grass.color) fieldlines_grass.color

all: $(TARGET1) $(TARGET2) $(TARGET3) $(TARGET4) $(TARGET5) $(TARGET6) $(TARGET7) $(TESTS)

clean:
	rm -f obj/*.o $(TARGET1) $(TARGET2) $(TARGET3) $(TARGET4) $(TARGET5) $(TARGET6) $(TARGET7) core *~ test_results/*

libclean:
	make -C ../../build clean

distclean: clean libclean

../../lib/darwin.a:
	make -C ../../build

obj/cblob.o: src/cblob.c
	$(C) $(CFLAGS) -c -o $@ $<

obj/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(TARGET1): ../../lib/darwin.a $(OBJECTS) obj/$(TARGET1).o
	$(CXX) $(CXXFLAGS) $(OBJECTS) obj/$(TARGET1).o ../../lib/darwin.a -o $(TARGET1) $(LFLAGS)
	chmod 755 $(TARGET1)

$(TARGET2): ../../lib/darwin.a $(OBJECTS) obj/$(TARGET2).o
	$(CXX) $(CXXFLAGS) $(OBJECTS) obj/$(TARGET2).o ../../lib/darwin.a -o $(TARGET2) $(LFLAGS)
	chmod 755 $(TARGET2)

$(TARGET3): ../../lib/darwin.a $(OBJECTS) obj/$(TARGET3).o
	$(CXX) $(CXXFLAGS) $(OBJECTS) obj/$(TARGET3).o ../../lib/darwin.a -o $(TARGET3) $(LFLAGS)
	chmod 755 $(TARGET3)

$(TARGET4): ../../lib/darwin.a $(OBJECTS) obj/$(TARGET4).o
	$(CXX) $(CXXFLAGS) $(OBJECTS) obj/$(TARGET4).o ../../lib/darwin.a -o $(TARGET4) $(LFLAGS)
	chmod 755 $(TARGET4)

$(TARGET5): ../../lib/darwin.a $(OBJECTS) obj/$(TARGET5).o
	$(CXX) $(CXXFLAGS) $(OBJECTS) obj/$(TARGET5).o ../../lib/darwin.a -o $(TARGET5) $(LFLAGS)
	chmod 755 $(TARGET5)

$(TARGET6): ../../lib/darwin.a $(OBJECTS) obj/$(TARGET6).o
	$(CXX) $(CXXFLAGS) $(OBJECTS) obj/$(TARGET6).o ../../lib/darwin.a -o $(TARGET6) $(LFLAGS)
	chmod 755 $(TARGET6)

$(TARGET7): ../../lib/darwin.a $(OBJECTS) obj/$(TARGET7).o
	$(CXX) $(CXXFLAGS) $(OBJECTS) obj/$(TARGET7).o ../../lib/darwin.a -o $(TARGET7) $(LFLAGS)
	chmod 755 $(TARGET7)

test_results/%_grass.color: images/%_grass.jpg $(TARGET3)
	./$(TARGET3) $< $@

test_results/case%.jpg: images/case%.jpg test_results/case%_grass.color $(TARGET3)
	./$(TARGET6) $< $(word 2, $^) $@

test_results/pole%.jpg: images/pole%.jpg test_results/pole%_grass.color $(TARGET7)
	./$(TARGET7) $< $(word 2, $^) $@

test_results/fieldcrossings.jpg: images/fieldlines.jpg $(TARGET2)
	./$(TARGET2) $< $@

test_results/fieldlines.jpg: images/fieldlines.jpg test_results/fieldlines_grass.color $(TARGET7)
	./$(TARGET7) $< $(word 2, $^) $@

