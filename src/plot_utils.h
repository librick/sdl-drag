#pragma once

#include <vector>
#include "point.h"

struct ImVec4;

struct Bounds {
    Point3D min;
    Point3D max;
};

void expand_bounds(Bounds& b, const std::vector<Point3D>& points);
void pad_range(double& lo, double& hi, double fraction, double min_half_width);
void plot_trajectory(const char* label, const std::vector<Point3D>& pos, int max_points, const ImVec4& color);
