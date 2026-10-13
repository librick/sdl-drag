CXXFLAGS := -std=c++17 -Wall -Wextra -O2 $(shell pkg-config --cflags sdl2)
LDLIBS := $(shell pkg-config --libs sdl2)

build/main: src/main.cpp src/color.h | build
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDLIBS)

build:
	mkdir -p build

clean:
	rm -rf build

run: build/main
	./build/main

.PHONY: clean run
