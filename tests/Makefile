COMPILER := gcc
CFLAGS := -Wall -Wextra -Wconversion -pedantic -std=gnu11 -I../include
SDL_FLGS := $(shell ../external/SDL/build/local/bin/sdl2-config --cflags --libs)

TEST_FILES := test.c ../src/cpu.c ../src/video.c ../src/stack.c

all: test run

test: $(TEST_FILES)
	$(COMPILER) $(CFLAGS) -o c8_test $(TEST_FILES) $(SDL_FLGS)

run:
	./c8_test
