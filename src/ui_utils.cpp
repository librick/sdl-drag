#include "ui_utils.h"
#include "imgui.h"
#include "simulation.h"
#include "plot_utils.h"
#include <implot3d.h>

bool slider_with_input(const char* label, double* v, double v_min, double v_max, const char* fmt) {
    const float label_width = ImGui::GetFontSize() * 9.0f;
    const float input_width = ImGui::GetFontSize() * 5.0f;
    const float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
    const float pad = ImGui::GetFontSize() * 0.5f;
    bool changed = false;

    ImGui::PushID(label);
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(label);
    ImGui::SameLine(label_width + pad);
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - input_width - spacing - pad);
    changed |= ImGui::SliderScalar("##slider", ImGuiDataType_Double, v, &v_min, &v_max, fmt);
    ImGui::SameLine(0.0f, spacing);
    ImGui::SetNextItemWidth(input_width);
    changed |= ImGui::InputDouble("##input", v, 0.0, 0.0, fmt);
    ImGui::PopID();
    return changed;
}

void table_row_key_value(const char* key, double value, const char* fmt) {
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::TextUnformatted(key);
    ImGui::TableSetColumnIndex(1);
    ImGui::Text(fmt, value);
}

void section_title(const char* text) {
    const ImGuiStyle& style = ImGui::GetStyle();
    ImGui::PushFont(nullptr, style.FontSizeBase * 1.2f);
    ImGui::PushStyleColor(ImGuiCol_Text, style.Colors[ImGuiCol_CheckMark]);
    ImGui::TextUnformatted(text);
    ImGui::PopStyleColor();
    ImGui::PopFont();
    ImGui::Separator();
    ImGui::Spacing();
}

void section_subtitle(const char* text) {
    const ImGuiStyle& style = ImGui::GetStyle();
    ImGui::Spacing();
    ImGui::PushStyleColor(ImGuiCol_Text, style.Colors[ImGuiCol_CheckMark]);
    ImGui::TextUnformatted(text);
    ImGui::PopStyleColor();
    ImGui::Separator();
    ImGui::Spacing();
}

void section_spacer() {
    ImGui::Dummy(ImVec2(0.0f, ImGui::GetFontSize() * 0.2f));
}

void section_end() {
    ImGui::Dummy(ImVec2(0.0f, ImGui::GetFontSize() * 1.0f));
}

bool section_presets(const std::vector<Preset>& presets, int& selected, SimulationParams& p) {
    const float label_width = ImGui::GetFontSize() * 9.0f;
    const float pad = ImGui::GetFontSize() * 0.5f;
    bool changed = false;
    const char* preview = "Custom";
    if (selected >= 0 && selected < (int)presets.size()) {
        preview = presets[selected].name;
    }
    section_title("Presets");
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Preset");
    ImGui::SameLine(label_width + pad);
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - pad);
    if (ImGui::BeginCombo("##preset", preview)) {
        for (int i = 0; i < (int)presets.size(); i++) {
            const bool is_selected = (i == selected);
            ImGui::PushID(i);
            if (ImGui::Selectable(presets[i].name, is_selected)) {
                selected = i;
                p = presets[i].params;
                changed = true;
            }
            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
            ImGui::PopID();
        }
        ImGui::EndCombo();
    }
    return changed;
}

bool section_controls(SimulationParams& p) {
    bool changed = false;
    section_title("Controls");
    changed |= slider_with_input("Mass m", &p.m, 0.01, 1000.0);
    changed |= slider_with_input("Drag constant C", &p.c, -10.0, 10.0);
    section_spacer();
    section_subtitle("Initial position");
    changed |= slider_with_input("x-position", &p.pos_initial.x, 0.0, 2000.0);
    changed |= slider_with_input("y-position", &p.pos_initial.y, 0.0, 2000.0);
    changed |= slider_with_input("z-position", &p.pos_initial.z, 0.0, 2000.0);
    section_spacer();
    section_subtitle("Initial velocity");
    changed |= slider_with_input("x-velocity", &p.vel_initial.x, -100.0, 100.0);
    changed |= slider_with_input("y-velocity", &p.vel_initial.y, -100.0, 100.0);
    changed |= slider_with_input("z-velocity", &p.vel_initial.z, -100.0, 100.0);
    section_spacer();
    section_subtitle("Initial acceleration");
    changed |= slider_with_input("x-acceleration", &p.acc_initial.x, -20.0, 20.0);
    changed |= slider_with_input("y-acceleration", &p.acc_initial.y, -20.0, 20.0);
    changed |= slider_with_input("z-acceleration", &p.acc_initial.z, -20.0, 20.0);
    section_spacer();
    section_end();
    return changed;
}

void section_stats(const char* title, const char* str_id, const SimulationParams& p, const SimulationResult& res) {
    Point3D pos_at_max_y = get_pos_at_max_y(res);
    ValueAndTime max_x = get_max_x(p, res);
    ValueAndTime max_y = get_max_y(p, res);
    ValueAndTime max_z = get_max_z(p, res);
    ValueAndTime min_x = get_min_x(p, res);
    ValueAndTime min_y = get_min_y(p, res);
    ValueAndTime min_z = get_min_z(p, res);
    ValueAndTime max_range = get_max_range(p, res);
    section_title(title);
    if (ImGui::BeginTable(str_id, 2)) {
        ImGui::TableSetupColumn("Property");
        ImGui::TableSetupColumn("Value");
        ImGui::TableHeadersRow();
        table_row_key_value("C/m", p.c / p.m);
        table_row_key_value("Max x", max_x.value);
        table_row_key_value("Max y", max_y.value);
        table_row_key_value("Max z", max_z.value);
        table_row_key_value("Min x", min_x.value);
        table_row_key_value("Min y", min_y.value);
        table_row_key_value("Min z", min_z.value);
        table_row_key_value("Time to reach max x", max_x.time);
        table_row_key_value("Time to reach max y", max_y.time);
        table_row_key_value("Time to reach max z", max_z.time);
        table_row_key_value("Time to reach min x", min_x.time);
        table_row_key_value("Time to reach min y", min_y.time);
        table_row_key_value("Time to reach min z", min_z.time);
        table_row_key_value("Value of x-position at apex", pos_at_max_y.x);
        table_row_key_value("Value of y-position at apex", pos_at_max_y.y);
        table_row_key_value("Value of z-position at apex", pos_at_max_y.z);
        table_row_key_value("Max range", max_range.value);
        table_row_key_value("Time to reach max range", max_range.time);
        ImGui::EndTable();
    }
    section_end();
}

void section_links() {
    section_title("Links");
    ImGui::TextLinkOpenURL("https://github.com/librick/sdl-drag");
    section_end();
}

void section_plot(
    const SimulationResult& result_drag,
    const SimulationResult& result_no_drag
) {
    static const ImVec4 color_no_drag = ImVec4(0.40f, 0.80f, 0.45f, 1.0f);
    static const ImVec4 color_drag = ImVec4(0.95f, 0.55f, 0.20f, 1.0f);
    if (ImGui::BeginChild("##plot_area", ImVec2(0, 0))) {
        Bounds b;
        b.min = result_drag.vector_pos.front();
        b.max = result_drag.vector_pos.front();
        expand_bounds(b, result_drag.vector_pos);
        expand_bounds(b, result_no_drag.vector_pos);
        pad_range(b.min.x, b.max.x, 0.05, 1.0);
        pad_range(b.min.y, b.max.y, 0.05, 1.0);
        pad_range(b.min.z, b.max.z, 0.05, 1.0);
        const ImPlot3DFlags plot_flags = ImPlot3DFlags_NoZoom | ImPlot3DFlags_NoPan;
        if (ImPlot3D::BeginPlot("Trajectory", ImVec2(-1, -1), plot_flags)) {
            ImPlot3D::SetupAxes("x", "z", "y");
            ImPlot3D::SetupAxesLimits(
                b.min.x, b.max.x,
                b.min.z, b.max.z,
                b.min.y, b.max.y,
                ImPlot3DCond_Always);

            plot_trajectory("No drag", result_no_drag.vector_pos, 300, color_no_drag);
            plot_trajectory("Drag", result_drag.vector_pos, 300, color_drag);

            ImPlot3D::EndPlot();
        }
    }
    ImGui::EndChild();
}
