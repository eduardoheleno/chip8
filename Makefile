COMPILER := gcc
CFLAGS := -Wall -Wextra -Wconversion -pedantic -std=gnu11 -I./include
SDL_FLGS := $(shell ./external/SDL/build/local/bin/sdl2-config --cflags --libs)

SRC_FILES := src/main.c src/stack.c src/cpu.c src/video.c src/keyboard.c

all: c8 run

sdl:
	cd ./external/SDL && \
	mkdir build && \
	cd build && \
	../configure --prefix=$(pwd)/local && \
	make && \
	make install

c8: $(SRC_FILES)
	$(COMPILER) $(CFLAGS) -o c8 $(SRC_FILES) $(SDL_FLGS)

run:
	./c8
