#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "stdlib.h"
#include <vector>
#include <string>
#include <map>
#include "meerkat_logger.hpp"
#include "meerkat_file_manager.hpp"
#include "meerkat_vector2.hpp"
#include "node.hpp"
#include "edge.hpp"

// Graph class
class Graph
{
private:
    std::map<std::string, Node> _nodes;
    std::vector<Edge> _edges;
    meerkat::mk_log _log;

    double _K;
    double _S;
    int _I;
    int _C;
    double _compat;
    double _sigma;
    double _weightThreshold;
    double _epsilon;
    double _beta;

    double _alpha;

public:
    Graph();
    void set_network_params(double weightThreshold_);
    void set_algorithm_params(double K_, double I0_, double compat_, double sigma_);
    void set_physics_params(double S0_, double epsilon_, double beta_);
    void set_graphics_params(double alpha_);
    int get_I();
    int get_C();
    void read(std::string nodesFile_, std::string edgesFile_);
    void get_bounding_box(meerkat::mk_vector2 &bottomLeft_,
                          meerkat::mk_vector2 &topRight_,
                          double frame_);
    void build_compatibility_lists();
    void iterate();
    void update_cycle();
    void smooth();
    void draw();
};

#endif // GRAPH_HPP
