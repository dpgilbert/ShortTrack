include Makefile.arch

SOURCES=$(wildcard *.cc) $(wildcard ../MT2CORE/*.cc) $(wildcard ../CORE/Tools/*.cc) $(wildcard ../CORE/Tools/MT2/*.cc) $(wildcard ../CORE/Tools/dorky/*.cc)

ROOTLIBS = $(shell root-config --ldflags --cflags --libs) -lTMVA -lGenVector

OBJECTS=$(SOURCES:.cc=.o)

LIB=libT1vsT5.so

EXE = T1vsT5.exe

$(LIB): $(OBJECTS)
	$(LD) $(LDFLAGS) $(SOFLAGS) $(OBJECTS) $(ROOTLIBS) -o $@

$(EXE): $(LIB) 
	echo "$(CXX) -o $@ $(LIB) $(ROOTLIBS) -Wl,-rpath,./"; \
	$(CXX) -o $@ $(LIB) $(ROOTLIBS) -Wl,-rpath,./	

%.o:  %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

all: 	$(EXE)
libs: 	$(LIB)
clean:
	rm -f *.o \
	rm -f *.d \
	rm -f *.so \
