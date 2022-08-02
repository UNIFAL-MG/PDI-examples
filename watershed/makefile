CC=gcc
#CCFLAGS=-Wall
CCFLAGS=-g
LDFLAGS=
SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)
TARGET=wshed

all: $(TARGET)

$(TARGET): $(OBJECTS);\
    $(CC) -g -o $@ ../utils/imagelib.o $^ $(LDFLAGS) 

%.o: %.c %.h;\
    $(CC) $(CCFLAGS) -c $<

%.o: %.c;\
    $(CC) $(CCFLAGS) -c $<

clean:;\
    rm -f *.o *-*.pgm $(TARGET);\