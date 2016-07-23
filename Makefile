CC=gcc
#CFLAGS= -ggdb -Wall -Wwrite-strings -Wmissing-declarations
CFLAGS= 
LDFLAGS= -lglut -lGL -lGLU -lm
TARGET=pelota
OBJS= main.o resources.o renderer.o physics.o mathutil.o data.o

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS)

main.o: main.c main.h data.h renderer.h resources.h mathutil.h physics.h

resources.o: resources.c data.h resources.h physics.h

renderer.o: renderer.c data.h renderer.h

physics.o: physics.c physics.h mathutil.h data.h

mathutil.o: mathutil.c mathutil.h

data.o: data.c data.h

.PHONY: clean

clean:
	rm $(TARGET) *.o
