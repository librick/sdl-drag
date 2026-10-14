#pragma once
#include <vector>
#include "point.h"

static const double g = 9.8;

struct SimulationParams {
    double dt = 0.01;
    double min_pos_y = 0.0;
    double c = 0.025;
    double m = 50.0;
    double t_initial = 0.0;
    double t_max = 300.0;
    Point3D pos_initial = { 0.0, 1000.0, 0.0 };
    Point3D vel_initial = { 0.0, 100.0, 0.0 };
    Point3D acc_initial = { 0.0, -g, 0.0 };
};

struct SimulationResult {
    std::vector<double> vector_t;
    std::vector<Point3D> vector_pos;
    std::vector<Point3D> vector_vel;
};

SimulationResult run_simulation(const SimulationParams& p);

struct ValueAndTime {
    double value;
    double time;
};

ValueAndTime get_max_x(const SimulationParams& p, const SimulationResult& res);
ValueAndTime get_max_y(const SimulationParams& p, const SimulationResult& res);
ValueAndTime get_max_z(const SimulationParams& p, const SimulationResult& res);
ValueAndTime get_max_range(const SimulationParams& p, const SimulationResult& res);

struct Preset {
    const char* name;
    SimulationParams params;
};

const std::vector<Preset>& simulation_presets();
