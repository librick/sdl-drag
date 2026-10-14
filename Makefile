CXXFLAGS := -std=c++17 -Wall -Wextra -O2
CPPFLAGS := $(shell pkg-config --cflags sdl2) -MMD -MP \
    -Iexternal/imgui \
	-Iexternal/imgui/backends \
	-Iexternal/implot3d \
    -DImDrawIdx="unsigned int"
LDLIBS  := $(shell pkg-config --libs sdl2)
SRCS_IMGUI := \
    $(wildcard external/imgui/*.cpp) \
    external/imgui/backends/imgui_impl_sdl2.cpp \
    external/imgui/backends/imgui_impl_sdlrenderer2.cpp
SRCS_IMPLOT3D := \
    external/implot3d/implot3d.cpp \
    external/implot3d/implot3d_items.cpp

SRCS := \
    src/main.cpp \
    src/simulation.cpp \
    src/ui_utils.cpp \
    src/plot_utils.cpp \
    $(SRCS_IMGUI) \
    $(SRCS_IMPLOT3D)
OBJS := $(SRCS:%.cpp=build/%.o)

build/main: $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

build/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -rf build

run: build/main
	./build/main

.PHONY: clean run

-include $(OBJS:.o=.d)
