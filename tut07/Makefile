CC=g++
LDLIBS=-lglut -lGLEW -lGL -lm -lSOIL

LOCAL_MODULE=suzanne

all: $(LOCAL_MODULE)
$(LOCAL_MODULE): shader_utils.o res_texture.o loader.o
clean:
	rm -f *.o $(LOCAL_MODULE)
.PHONY: all clean


