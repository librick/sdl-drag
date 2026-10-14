#include <cmath>
#include "point.h"
#include "simulation.h"

SimulationResult run_simulation(const SimulationParams& p) {
    SimulationResult res;

    // Simulation state
    double t = p.t_initial;
    Point3D pos = p.pos_initial;
    Point3D vel = p.vel_initial;
    const Point3D acc = p.acc_initial;
    Point3D acc_drag = { 0.0, 0.0, 0.0 };

    res.vector_t.push_back(t);
    res.vector_pos.push_back(pos);
    res.vector_vel.push_back(vel);

    while(t < p.t_max) {
        /**
         * F=Cv^2, where:
         *  F is the magnitude of the force due to drag
         *  C is some constant
         *  v is the speed of the object
         * In vector form:
         *  F_{overarrow} points opposite v_{overarrow}
         *  F_{overarrow} = Cv^2 * (-v_{hat})
         *  F_{overarrow} = Cv^2 * -v_{overarrow}/v
         *  F_{overarrow} = -Cv*v_{overarrow}
         * Then, from Newton's second law, F_{overarrow} = m * a_{overarrow}:
         *  a_x = -Cv*v_x/m
         *  a_y = -Cv*v_y/m
         *  a_z = -Cv*v_z/m
         */
        double speed = std::sqrt(vel.x*vel.x + vel.y*vel.y + vel.z*vel.z);
        acc_drag.x = -(p.c*speed*vel.x)/p.m;
        acc_drag.y = -(p.c*speed*vel.y)/p.m;
        acc_drag.z = -(p.c*speed*vel.z)/p.m;
        // Update position
        pos.x = (pos.x) + (vel.x*p.dt) + (0.5*(acc.x+acc_drag.x)*p.dt*p.dt);
        pos.y = (pos.y) + (vel.y*p.dt) + (0.5*(acc.y+acc_drag.y)*p.dt*p.dt);
        pos.z = (pos.z) + (vel.z*p.dt) + (0.5*(acc.z+acc_drag.z)*p.dt*p.dt);
        // If our new position vector is beyond range, break
        if(pos.y < p.min_pos_y) {
            break;
        }
        // Update velocity
        vel.x = (vel.x) + (acc.x+acc_drag.x)*p.dt;
        vel.y = (vel.y) + (acc.y+acc_drag.y)*p.dt;
        vel.z = (vel.z) + (acc.z+acc_drag.z)*p.dt;
        // Update time
        t = t + p.dt;
        // Persist state
        res.vector_pos.push_back(pos);
        res.vector_vel.push_back(vel);
        res.vector_t.push_back(t);
    }
    return res;
}

ValueAndTime get_max_x(const SimulationParams& p, const SimulationResult& res) {
    double max_x = -1.0;
    double t_at_max = -1.0;
    for(unsigned long i = 0; i < res.vector_pos.size(); i++) {
        const Point3D pos = res.vector_pos.at(i);
        if(pos.x > max_x) {
            max_x = pos.x;
            t_at_max = p.t_initial + i*p.dt;
        }
    }
    return { max_x, t_at_max };
}

ValueAndTime get_max_y(const SimulationParams& p, const SimulationResult& res) {
    double max_y = -1.0;
    double t_at_max = -1.0;
    for(unsigned long i = 0; i < res.vector_pos.size(); i++) {
        const Point3D pos = res.vector_pos.at(i);
        if(pos.y > max_y) {
            max_y = pos.y;
            t_at_max = p.t_initial + i*p.dt;
        }
    }
    return { max_y, t_at_max };
}

ValueAndTime get_max_z(const SimulationParams& p, const SimulationResult& res) {
    double max_z = -1.0;
    double t_at_max = -1.0;
    for(unsigned long i = 0; i < res.vector_pos.size(); i++) {
        const Point3D pos = res.vector_pos.at(i);
        if(pos.z > max_z) {
            max_z = pos.z;
            t_at_max = p.t_initial + i*p.dt;
        }
    }
    return { max_z, t_at_max };
}

ValueAndTime get_max_range(const SimulationParams& p, const SimulationResult& res) {
    double max_range = 0.0;
    double t_at_max = 0.0;
    for(unsigned long i = 0; i < res.vector_pos.size(); i++) {
        const Point3D pos = res.vector_pos.at(i);
        const double candidate_range = std::sqrt(pos.x*pos.x + pos.z*pos.z);
        if(candidate_range > max_range) {
            max_range = candidate_range;
            t_at_max = p.t_initial + i*p.dt;
        }
    }
    return { max_range, t_at_max };
}

static std::vector<Preset> make_presets() {
    SimulationParams base;
    base.pos_initial = { 0.0, 1000.0, 0.0 };
    base.acc_initial = { 0.0, -g, 0.0 };
    base.c = 0.025;
    base.m = 50.0;

    SimulationParams p1 = base;
    p1.vel_initial = { 0.0, 100.0, 0.0 };

    SimulationParams p2 = base;
    p2.vel_initial = { 50.0, 50.0, 0.0 };

    return {
        { "Preset 1", p1 },
        { "Preset 2", p2 },
    };
}

const std::vector<Preset>& simulation_presets() {
    static const std::vector<Preset> presets = make_presets();
    return presets;
}
