CC=g++
LDLIBS=-lglut -lGLEW -lGL -lm

LOCAL_MODULE=cube

all: $(LOCAL_MODULE)
$(LOCAL_MODULE): shader_utils.o
clean:
	rm -f *.o $(LOCAL_MODULE)
.PHONY: all clean


