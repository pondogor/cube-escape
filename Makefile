# Makefile for Cube Escape

CC     = gcc
CFLAGS = $(shell sdl-config --cflags)
LIBS   = $(shell sdl-config --libs) -lm -lSDL_ttf -lSDL_image

TARGET  = cube-escape

HEADERS = src/main.h \
          src/states.h \
          src/draw.h \
          src/maze.h \
          src/types.h \

OBJS    = src/main.o \
          src/states.o \
          src/draw.o \
          src/maze.o \

$(TARGET):  $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LIBS)

src/main.o:  src/main.c $(HEADERS)

src/states.o:  src/states.c $(HEADERS)

src/draw.o:  src/draw.c $(HEADERS)

src/maze.o:  src/maze.c $(HEADERS)

clean-objs:
	rm -f $(OBJS)

clean:
	rm -f $(TARGET) $(OBJS)
