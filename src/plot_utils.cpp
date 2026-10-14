#include "plot_utils.h"
#include <algorithm>
#include <vector>
#include <implot3d.h>

void expand_bounds(Bounds& b, const std::vector<Point3D>& points) {
    for (const Point3D& p : points) {
        b.min.x = std::min(b.min.x, p.x);
        b.min.y = std::min(b.min.y, p.y);
        b.min.z = std::min(b.min.z, p.z);
        b.max.x = std::max(b.max.x, p.x);
        b.max.y = std::max(b.max.y, p.y);
        b.max.z = std::max(b.max.z, p.z);
    }
}

void pad_range(double& lo, double& hi, double fraction, double min_half_width) {
    double half = (hi - lo) * 0.5 * (1.0 + fraction);
    if (half < min_half_width) {
        half = min_half_width;
    }
    const double mid = (hi + lo) * 0.5;
    lo = mid - half;
    hi = mid + half;
}

void plot_trajectory(const char* label, const std::vector<Point3D>& pos, int max_points, const ImVec4& color) {
    const int total = (int)pos.size();
    int step = total / max_points;
    if (step < 1) {
        step = 1;
    }
    const int count = (total + step - 1) / step;
    ImPlot3DSpec spec;
    spec.Stride = (int)(sizeof(Point3D) * step);
    spec.Marker = ImPlot3DMarker_Circle;
    spec.MarkerSize = 2.0f;
    spec.LineColor = color;
    spec.MarkerLineColor = color;
    spec.MarkerFillColor = color;
    const Point3D* p = pos.data();
    ImPlot3D::PlotScatter(label, &p->x, &p->z, &p->y, count, spec);
}