#include <string>
#include <SDL2/SDL.h>
#include "color.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include <implot3d.h>
#include "simulation.h"
#include <algorithm>
#include "ui_utils.h"
#include "plot_utils.h"

static const int window_width = 800;
static const int window_height = 480;
static const std::string title = "SDL Drag Demo";
static const SDL_Color background_color = color_from_hex(0x1970fc);

int main()
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    // Create SDL window
    SDL_Window *window = SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        window_width,
        window_height,
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);
    if (window == nullptr)
    {
        std::fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Create SDL renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr)
    {
        std::fprintf(stderr, "SDL_CreateRenderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot3D::CreateContext();
    ImGui::StyleColorsDark();

    if (!ImGui_ImplSDL2_InitForSDLRenderer(window, renderer))
    {
        std::fprintf(stderr, "ImGui_ImplSDL2_InitForSDLRenderer failed\n");
        return 1;
    }
    if (!ImGui_ImplSDLRenderer2_Init(renderer))
    {
        std::fprintf(stderr, "ImGui_ImplSDLRenderer2_Init failed\n");
        return 1;
    }

    // Use better font
    ImGui::GetIO().Fonts->AddFontFromFileTTF("external/imgui/misc/fonts/Roboto-Medium.ttf");
    ImGui::GetStyle().FontSizeBase = 20.0f;

    const std::vector<Preset>& presets = simulation_presets();
    int preset_index = 1;
    SimulationParams params_controls = presets[preset_index].params;

    // Start the game loop
    bool running = true;
    while (running)
    {
        // Handle quit event
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
            {
                // Stop the game loop
                running = false;
            }
        }
        
        // Set each simulations parameters from the controls
        SimulationParams params_drag = params_controls;
        SimulationParams params_no_drag = params_controls;
        params_no_drag.c = 0.0;

        // Run simulations
        const SimulationResult result_drag = run_simulation(params_drag);
        const SimulationResult result_no_drag = run_simulation(params_no_drag);

        ImGui_ImplSDLRenderer2_NewFrame();
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
                section_presets(presets, preset_index, params_controls);
                if (section_controls(params_controls)) {
                    preset_index = -1;
                }
                section_stats("Simulation without drag", "##stats", params_no_drag, result_no_drag);
                section_stats("Simulation with drag", "##stats_drag", params_drag, result_drag);
            }
            ImGui::EndChild();
            ImGui::SameLine();
            section_plot(result_drag, result_no_drag);
        }
        ImGui::End();

        ImGui::Render();
        SDL_SetRenderDrawColor(
            renderer,
            background_color.r,
            background_color.g,
            background_color.b,
            255);
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
        SDL_RenderPresent(renderer);
    }

    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImPlot3D::DestroyContext();
    ImGui::DestroyContext();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
