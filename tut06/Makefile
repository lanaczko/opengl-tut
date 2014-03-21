CC=g++
LDLIBS=-lglut -lGLEW -lGL -lm -lSOIL

LOCAL_MODULE=cube
LOCAL_MODULE+=cube_soil

all: $(LOCAL_MODULE)
$(LOCAL_MODULE): shader_utils.o res_texture.o
clean:
	rm -f *.o $(LOCAL_MODULE)
.PHONY: all clean


