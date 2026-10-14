#include <string>
#include <cstdio>
#include <vector>
#include <SDL2/SDL.h>
#include <implot3d.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

#ifdef __EMSCRIPTEN__
#include <SDL2/SDL_opengles2.h>
#else
#include <SDL2/SDL_opengl.h>
#endif

#include "color.h"
#include "simulation.h"
#include "ui_utils.h"
#include "plot_utils.h"

static const int window_width = 800;
static const int window_height = 480;
static const std::string title = "SDL Drag Demo";
static const SDL_Color background_color = color_from_hex(0x1970fc);

struct AppState {
    SDL_Window* window = nullptr;
    const std::vector<Preset>* presets = nullptr;
    int preset_index = 1;
    SimulationParams params_controls;
    bool running = true;
};

static void frame(AppState& s) {
    // Handle quit event
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT)
        {
            // Stop the game loop
            s.running = false;
        }
    }
    
    // Set each simulations parameters from the controls
    SimulationParams params_drag = s.params_controls;
    SimulationParams params_no_drag = s.params_controls;
    params_no_drag.c = 0.0;

    // Run simulations
    const SimulationResult result_drag = run_simulation(params_drag);
    const SimulationResult result_no_drag = run_simulation(params_no_drag);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    const ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGuiWindowFlags host_window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
                                            ImGuiWindowFlags_NoSavedSettings;

    if (ImGui::Begin("Main", nullptr, host_window_flags)) {
        const float sidebar_width = ImGui::GetFontSize() * 24.0f;
        if(ImGui::BeginChild("##sidebar", ImVec2(sidebar_width, 0), ImGuiChildFlags_ResizeX)) {
            section_presets(*s.presets, s.preset_index, s.params_controls);
            if (section_controls(s.params_controls)) {
                s.preset_index = -1;
            }
            section_stats("Simulation without drag", "##stats", params_no_drag, result_no_drag);
            section_stats("Simulation with drag", "##stats_drag", params_drag, result_drag);
            section_links();
        }
        ImGui::EndChild();
        ImGui::SameLine();
        section_plot(result_drag, result_no_drag);
    }
    ImGui::End();

    ImGui::Render();
    const ImGuiIO& io = ImGui::GetIO();
    glViewport(
        0,
        0,
        (int)(io.DisplaySize.x * io.DisplayFramebufferScale.x),
        (int)(io.DisplaySize.y * io.DisplayFramebufferScale.y));
    glClearColor(
        background_color.r / 255.0f,
        background_color.g / 255.0f,
        background_color.b / 255.0f,
        1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(s.window);
}

#ifdef __EMSCRIPTEN__
static void frame_callback(void* arg) {
    frame(*static_cast<AppState*>(arg));
}
#endif

int main()
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

#ifdef __EMSCRIPTEN__
    const char* glsl_version = "#version 100";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#else
    const char* glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // Create SDL window
    SDL_Window *window = SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        window_width,
        window_height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED | SDL_WINDOW_ALLOW_HIGHDPI);
    if (window == nullptr)
    {
        std::fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    if (gl_context == nullptr)
    {
        std::fprintf(stderr, "SDL_GL_CreateContext failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
#ifdef __EMSCRIPTEN__
    ImGui::GetIO().IniFilename = nullptr;
    SDL_SetHint(SDL_HINT_EMSCRIPTEN_KEYBOARD_ELEMENT, "#canvas");
#endif
    ImPlot3D::CreateContext();
    ImGui::StyleColorsDark();

    if (!ImGui_ImplSDL2_InitForOpenGL(window, gl_context))
    {
        std::fprintf(stderr, "ImGui_ImplSDL2_InitForOpenGL failed\n");
        return 1;
    }
    if (!ImGui_ImplOpenGL3_Init(glsl_version))
    {
        std::fprintf(stderr, "ImGui_ImplOpenGL3_Init failed\n");
        return 1;
    }

    // Use better font
    ImGui::GetIO().Fonts->AddFontFromFileTTF("external/imgui/misc/fonts/Roboto-Medium.ttf");
#ifdef __EMSCRIPTEN__
    ImGui::GetStyle().FontSizeBase = 16.0f;
#else
    ImGui::GetStyle().FontSizeBase = 20.0f;
#endif

    static AppState state;
    state.window = window;
    state.presets = &simulation_presets();
    state.params_controls = (*state.presets)[state.preset_index].params;

    // Start main loop
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(frame_callback, &state, 0, true);
#else
    while (state.running) {
        frame(state);
    }
#endif

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImPlot3D::DestroyContext();
    ImGui::DestroyContext();
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}