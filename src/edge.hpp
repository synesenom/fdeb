#ifndef EDGE_HPP
#define EDGE_HPP

#include <vector>
#include "meerkat_vector2.hpp"
#include "GLUT/glut.h"
#include"math.h"
#include <algorithm>

#define EPSILON 1e-6
#define INV_SQRT_2PI 0.3989422804

meerkat::mk_vector2 center(meerkat::mk_vector2 &p1_, meerkat::mk_vector2 &p2_);
meerkat::mk_vector2 project(meerkat::mk_vector2 &point_,
                            meerkat::mk_vector2 &lineStart_,
                            meerkat::mk_vector2 &lineEnd_);
double gauss_weight(int dist_, double sigma_);

struct Edge
{
    // variables
    meerkat::mk_vector2 _start;
    meerkat::mk_vector2 _end;
    std::vector<meerkat::mk_vector2> _subdivs;
    double _width;
    std::vector<int> _compatibleEdges;
    std::vector<double> _compatibilities;

    // edge manipulation
    void add_subdivisions();
    void arrange_direction();
    void update(std::vector<meerkat::mk_vector2> &forces_, double S_);
    void smooth(double sigma_);

    // forces
    void add_spring_forces(std::vector<meerkat::mk_vector2> &forces_, double K_);
    void add_electrostatic_forces(std::vector<meerkat::mk_vector2> &forces_,
                                  Edge edge_, double epsilon_);
    void add_gravitational_forces(std::vector<meerkat::mk_vector2> &forces_,
                                  meerkat::mk_vector2 &center_,
                                  double beta_ );

    // helper methods
    meerkat::mk_vector2 vector();

    // draw
    void draw(double alpha_);

    // constructors
    Edge();
};

double angle_compatilibity(Edge &edge1_, Edge &edge2_);
double scale_compatibility(Edge &edge1_, Edge &edge2_);
double position_compatibility(Edge &edge1_, Edge &edge2_);
double edge_visibility(Edge &edge1_, Edge &edge2_);
double visibility_compability(Edge &edge1_, Edge &edge2_);

#endif // EDGE_HPP
