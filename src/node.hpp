#ifndef NODE_HPP
#define NODE_HPP

#include "meerkat_vector2.hpp"
#include "GLUT/glut.h"

#define NUM_SEGMENTS 16
#define COS_THETA 0.92387953251
#define SIN_THETA 0.38268343236
#define RADIUS 1.0

// Node struct
struct Node
{
    // Variables
    meerkat::mk_vector2 _pos;       // Position of node.
    int _degree;                    // Degree of node.

    /**
     * @brief Node Empty constructor.
     * Sets position to (0, 0) and degree to 0.
     */
    Node();

    /**
     * @brief Node Constructor with coordinates.
     * @param x_ X coordinate.
     * @param y_ Y coordinate.
     */
    Node(double x_, double y_);

    /**
     * @brief draw Draws the node.
     */
    void draw();
};

#endif // NODE_HPP
