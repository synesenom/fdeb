#ifndef NODE_HPP
#define NODE_HPP

#include "meerkat_vector2.hpp"
#include "GLUT/glut.h"

#define NUM_SEGMENTS 16
#define COS_THETA 0.92387953251
#define SIN_THETA 0.38268343236
#define RADIUS 1.0

struct Node
{
    meerkat::mk_vector2 _pos;
    int _degree;

    Node();
    Node(double x_, double y_);
    void draw();
};

#endif // NODE_HPP
