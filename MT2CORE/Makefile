include Makefile.arch

SOURCES=$(wildcard *.cc) $(wildcard MT2/*.cc) $(wildcard Plotting/*.cc)

OBJECTS=$(SOURCES:.cc=.o)

LIB=libMT2CORE.so

$(LIB): $(OBJECTS)
	$(LD) $(LDFLAGS) $(SOFLAGS) $(OBJECTS) $(ROOTLIBS) -o $@

%.o:  %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

all: $(LIB) 
clean:
	rm -f *.o \ 
	rm -f *.d \
	rm -f *.so \
	rm -f MT2/*.o \ 
	rm -f MT2/*.d \
	rm -f MT2/*.so \
	rm -f Plotting/*.o \ 
	rm -f Plotting/*.d \
	rm -f Plotting/*.so
