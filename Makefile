CXXFLAGS := -std=c++17 -Wall -Wextra -O2
CPPFLAGS := $(shell pkg-config --cflags sdl2) -MMD -MP \
    -Iexternal/imgui \
	-Iexternal/imgui/backends \
	-Iexternal/implot3d \
    -DImDrawIdx="unsigned int"
LDLIBS  := $(shell pkg-config --libs sdl2) -lGL
SRCS_IMGUI := \
    $(wildcard external/imgui/*.cpp) \
    external/imgui/backends/imgui_impl_sdl2.cpp \
    external/imgui/backends/imgui_impl_opengl3.cpp
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

WEB_CPPFLAGS := $(filter-out $(shell pkg-config --cflags sdl2),$(CPPFLAGS))
WEB_FLAGS := -sUSE_SDL=2
WEB_LDFLAGS := $(WEB_FLAGS) -O2 \
    -sALLOW_MEMORY_GROWTH=1 \
    --preload-file external/imgui/misc/fonts/Roboto-Medium.ttf \
    --shell-file external/imgui/examples/libs/emscripten/shell_minimal.html
WEB_OBJS := $(SRCS:%.cpp=build-web/%.o)

build-web/index.html: $(WEB_OBJS)
	em++ -o $@ $^ $(WEB_LDFLAGS)

build-web/%.o: %.cpp
	@mkdir -p $(dir $@)
	em++ $(WEB_CPPFLAGS) $(CXXFLAGS) $(WEB_FLAGS) -c -o $@ $<

web: build-web/index.html

serve: web
	python3 -m http.server -d build-web 8000

clean:
	rm -rf build build-web

run: build/main
	./build/main

.PHONY: clean run web serve

-include $(OBJS:.o=.d) $(WEB_OBJS:.o=.d)
