#pragma once
#include <vector>

struct SimulationParams;
struct SimulationResult;
struct Preset;

bool slider_with_input(const char* label, double* v, double v_min, double v_max, const char* fmt = "%.4f");
void table_row_key_value(const char* key, double value);

void section_title(const char* text);
void section_subtitle(const char* text);
void section_spacer();
void section_end();

bool section_presets(const std::vector<Preset>& presets, int& selected, SimulationParams& p);
bool section_controls(SimulationParams& p);
void section_stats(const char* title, const char* str_id, const SimulationParams& p, const SimulationResult& res);
void section_plot(const SimulationResult& result_drag, const SimulationResult& result_no_drag);
