CC = g++
INCLUDE = -I../ -I./ 
CFLAGS = -Wall -g -fPIC $(shell root-config --cflags) $(INCLUDE) $(EXTRACFLAGS) -DTOOLSLIB
LINKER = g++

LINKERFLAGS = $(shell root-config --ldflags) $(shell root-config --libs) -lMinuit
ifeq ($(shell root-config --platform),macosx)
	LINKERFLAGS = -dynamiclib -undefined dynamic_lookup -Wl,-x -O -Xlinker -bind_at_load -flat_namespace $(shell root-config --libs) -lEG -lGenVector
endif

SOURCES =  MT2Looper.cc 
OBJECTS = $(SOURCES:.cc=.o) LinkDef_out.o
LIB = libMT2Looper.so

CORELIB = libMT2CORE.so
#CORELIB = ../MT2CORE/libMT2CORE.so

FWLIB = libMiniFWLite.so
#FWLIB = ../Tools/MiniFWLite/libMiniFWLite.so

LIBS = $(LIB) $(CORELIB) $(FWLIB) 

.PHONY: all help compile clean cms2env

libs:	$(LIBS)

$(LIB):	$(OBJECTS) 
	echo "Linking $(LIB)"; \
	$(LINKER) $(LINKERFLAGS) -shared $(OBJECTS) -o $@

$(CORELIB):
	echo "making MT2CORE"; \
        cd ../MT2CORE; \
        $(MAKE) -f Makefile; cd -; \
	cp ../MT2CORE/$(CORELIB) .;

$(FWLIB):
	echo "making MiniFWLite"; \
        cd ../Tools/MiniFWLite; \
        $(MAKE) -f Makefile; cd -; \
	cp ../Tools/MiniFWLite/$(FWLIB) .;

LinkDef_out.cxx: LinkDef.h MT2Looper.h 
	rootcint -f $@ -c $(INCLUDE) MT2Looper.h  $<

# General rule for making object files
%.d:	%.cc
	$(CC) -MM -MT $@ -MT ${@:.d=.o} $(CFLAGS) $< > $@; \
                     [ -s $@ ] || rm -f $@
%.d:	%.cxx
	$(CC) -MM -MT $@ -MT ${@:.d=.o} $(CFLAGS) $< > $@; \
                     [ -s $@ ] || rm -f $@

%.o: 	%.cc 
	$(CC) $(CFLAGS) $< -c -o $@

%.o: 	%.cxx
	$(CC) $(CFLAGS) $< -c -o $@

.PHONY: all
all:	$(LIBS)  

.PHONY: clean
clean:  
	rm -v -f \
        ../MT2CORE/*.o \
        ../MT2CORE/*.d \
        ../MT2CORE/*.so \
        ../Tools/*.o \
	../Tools/*.d \
        ../Tools/MiniFWLite/*.o \
	../Tools/MiniFWLite/*.d \
        $(CORELIB) $(LIB) $(FWLIB) \
        LinkDef_out.cxx  LinkDef_out.h \
        *.o *.d *.so; echo "Done"

test: 
	@echo $(INCLUDE)

-include $(LIBDIR)/LinkDef_out.d

