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
    // Network structure
    std::map<std::string, Node> _nodes;
    std::vector<Edge> _edges;

    // Logger
    meerkat::mk_log _log;

    // Algorithm parameters
    double _K;                                  // Global spring constant (K).
    int _I;                                     // Number of iterations in cycle.
    int _iter;                                  // Number of remaining iterations.
    int _cycles;                                // Cycles left;
    double _compatibilityThreshold;             // Compatibility threshold.
    double _smoothWidth;                        // Width of the Gaussian smoothing.

    // Physical parameters
    double _S;                                  // Displacement of division points in a single iteration.
    double _edgeDistance;                       // Minimum distance between edges.
    bool _gravitationIsOn;                      // Marks whether gravitation is on.
    meerkat::mk_vector2 _gravitationCenter;     // Gravitation center.
    double _gravitationExponent;                // Gravitation exponent.

    // Network parameters
    double _edgeWeightThreshold;                // Threshold on edge weights (for dense graphs).
    double _edgePercentageThreshold;            // Percentage of edges being kept (for dense graphs).

    // Graphics parameters
    double _edgeOpacity;                        // Opacity.

public:
    /**
     * @brief Graph Constructor.
     * Sets default parameter values.
     */
    Graph();

    /**
     * @brief set_network_params Sets network parameters.
     * @param weightThreshold_ Threshold of edge weights.
     */
    void set_network_params(double edgeWeightThreshold_, double edgePercentageThreshold_);

    /**
     * @brief set_algorithm_params Sets algorithm parameters.
     * @param K_      Global spring constant.
     * @param cycles_ Total number of cycles.
     * @param I0_     Initial number of iterations.
     * @param compat_ Compatibility threshold.
     * @param sigma_  Bandwidth of Gaussian kernel.
     */
    void set_algorithm_params(double K_, int cycles_, int I0_, double compat_, double sigma_);

    /**
     * @brief set_physics_params Sets phyisical parameters.
     * @param S0_           Single displacement.
     * @param edgeDistance_ Minimum edge distance.
     * @param gravCenter_   Center of gravitation.
     * @param gravExponent_ Exponent of gravitation.
     */
    void set_physics_params(double S0_, double edgeDistance_,
                            meerkat::mk_vector2 gravCenter_,
                            double gravExponent_);

    /**
     * @brief enable_gravitation Sets gravitation on.
     */
    void enable_gravitation();

    /**
     * @brief set_graphics_params Sets graphical parameters fro visualization.
     * @param alpha_ Opacity of edges.
     */
    void set_graphics_params(double alpha_);

    /**
     * @brief read Reads in a network (node coordinates and edges).
     * @param nodesFile_ Name of the node coordinates file.
     * @param edgesFile_ Name of the edge endpoints file.
     */
    void read(std::string nodesFile_, std::string edgesFile_);

    /**
     * @brief get_bounding_box Calculates bounding box of the network.
     * @param bottomLeft_ Bottom left corner.
     * @param topRight_   Top right corner.
     * @param frame_      Frame width around the network.
     */
    void get_bounding_box(meerkat::mk_vector2 &bottomLeft_,
                          meerkat::mk_vector2 &topRight_,
                          double frame_);

    /**
     * @brief build_compatibility_lists Builds compatibility lists for edges.
     */
    void build_compatibility_lists();

    /**
     * @brief iterate Performs a single iteration.
     * @return Number of iterations left.
     */
    int iterate();

    /**
     * @brief update_cycle Updates cycle.
     * This reduces S, I and increases C.
     * @return Current cycle index.
     */
    int update_cycle();

    /**
     * @brief add_subvisions Adds subdivision points.
     */
    void add_subvisions();

    /**
     * @brief smooth Performs Gaussian smooth of the edges.
     */
    void smooth();

    /**
     * @brief draw Draws the network.
     */
    void draw();

    /**
     * @brief print_json Prints network in JSON format.
     * @param output_ Name of output file.
     */
    void print_json(std::string output_);

    /**
     * @brief print_svg Prints network in SVG format.
     * @param output_ Name of output file.
     */
    void print_svg(std::string output_);
};

#endif // GRAPH_HPP
