# try to do some autodetecting
UNAME := $(shell uname -s)

ifeq "$(UNAME)" "Darwin"
	OS=macosx
endif
ifeq "$(OS)" "Windows_NT"
	OS=windows
endif
ifeq "$(UNAME)" "Linux"
	OS=linux
endif


#################  Mac OS X  ##################################################
ifeq "$(OS)" "macosx"

EXE_SUFFIX=

ARCHS=   -arch i386 -arch x86_64
CFLAGS+= $(ARCHS)
CFLAGS += -mmacosx-version-min=10.6
CFLAGS_MONGOOSE=  -I./mongoose -pthread -g 
LIBS+=	 $(ARCHS)

endif

#################  Windows  ##################################################
ifeq "$(OS)" "windows"

EXE_SUFFIX=.exe

CFLAGS_MONGOOSE = -I./mongoose -mthreads

endif


#################  Common  ##################################################

CFLAGS += $(INCLUDES) -O -Wall -std=gnu99


all: cloudpoint

cloudpoint: cloudpoint.o arduino-serial-lib.o
	$(CC) $(CFLAGS) -o cloudpoint$(EXE_SUFFIX) cloudpoint.o arduino-serial-lib.o $(LIBS)

.c.o:
	$(CC) $(CFLAGS) -c $*.c -o $*.o

clean:
	rm -f $(OBJ) arduino-serial arduino-serial.exe *.o *.a

