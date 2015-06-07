#include "node.hpp"

Node::Node()
{
    _pos.set(0.0, 0.0);
    _degree = 0;
}

Node::Node(double x_, double y_)
{
    _pos.set(x_, y_);
    _degree = 0;
}

void Node::draw()
{
    double x = RADIUS, y = 0.0, t;
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    if( _degree == 0 )
        glColor3f( 0.95, 0.95, 0.95 );
    else
        glColor3f( 0./255., 180./255., 240./255. );
    glBegin( GL_TRIANGLE_FAN );
    glVertex2d( _pos.x(), _pos.y() );
    for( int i=0; i<=NUM_SEGMENTS; i++ )
    {
        glVertex2d( x + _pos.x(), y + _pos.y() );
        t = x;
        x = COS_THETA * x - SIN_THETA * y;
        y = SIN_THETA * t + COS_THETA * y;
    }
    glEnd();
}
