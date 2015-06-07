#include "stdlib.h"
#include "graph.hpp"
#include "GLUT/glut.h"
#include "meerkat_argument_manager.hpp"
#include "meerkat_vector2.hpp"

#define DEFAULT_SIZE 600

Graph gGraph;
int gMaxCycles = 6;

void init( meerkat::mk_vector2 bottomLeft_,
           meerkat::mk_vector2 topRight_ )
{
    glClearColor( 1.0, 1.0, 1.0, 1.0 );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D( bottomLeft_.x(), topRight_.x(), bottomLeft_.y(), topRight_.y() );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    glEnable( GL_LINE_SMOOTH );
    glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable( GL_BLEND );
    gGraph.iterate();
}

void display()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    gGraph.draw();
    glutSwapBuffers();
    glFlush();
}

void keyboard( unsigned char key_, int x_, int y_ )
{
    switch( key_ )
    {
    case 27:
        exit(0);
        break;
    }
}

void timer( int var_ )
{
    static int i = gGraph.get_I();
    if( i > 0 )
    {
        gGraph.iterate();
        i--;
    }
    if( i == 0 )
    {
        gGraph.update_cycle();
        i = gGraph.get_I();
        if( gGraph.get_C() == gMaxCycles+1 )
        {
            i = -1;
            gGraph.smooth();
            display();
            return;
        }
    }
    display();
    glutTimerFunc( 100, timer, var_ );
}

int main( int argc_, char **argv_ )
{
    meerkat::mk_argument_manager a("Force-directed edge bundling calculator",
                                   "Usage: fdeb [options]");
    a.add_help();
    a.add_argument_entry( "nodes", MK_VALUE, "--nodes", "-n",
                          "File containing node positions", "", MK_REQUIRED);
    a.add_argument_entry( "edges", MK_VALUE, "--edges", "-e",
                          "File containing edges", "", MK_REQUIRED);
    a.add_argument_entry( "K", MK_VALUE, "--K", "-K",
                          "Edge stiffness [0.1]", "0.1", MK_OPTIONAL);
    a.add_argument_entry( "S", MK_VALUE, "--S", "-S",
                          "Initial value of S [0.1]", "0.1", MK_OPTIONAL);
    a.add_argument_entry( "I", MK_VALUE, "--I", "-i",
                          "Initial number of iterations [90]", "90", MK_OPTIONAL);
    a.add_argument_entry( "compat", MK_VALUE, "--compat", "-c",
                          "Compatibility threshold [0.05]", "0.05", MK_OPTIONAL);
    a.add_argument_entry( "cycles", MK_VALUE, "--cycles", "-C",
                          "Max number of cycles [8]", "8", MK_OPTIONAL);
    a.add_argument_entry( "sigma", MK_VALUE, "--sigma", "-s",
                          "Smoothing width [30]", "30", MK_OPTIONAL);
    a.add_argument_entry( "weight threshold", MK_VALUE, "--weight-threshold", "-w",
                          "Edge weight threshold [0.0]", "0.0", MK_OPTIONAL);
    a.add_argument_entry( "epsilon", MK_VALUE, "--epsilon", "-E",
                          "Lowest interaction distance [1e-4]", "0.0001", MK_OPTIONAL);
    a.add_argument_entry( "gravitation exponent", MK_VALUE, "--gravitation-exponent", "-ge",
                          "Gravitation exponent [1.0]", "1.0", MK_OPTIONAL);
    a.add_argument_entry( "gravitation center x", MK_VALUE, "--gravitation-center-x", "-gcx",
                          "Gravitation center x coordinate [0.0]", "0.0", MK_OPTIONAL);
    a.add_argument_entry( "gravitation center y", MK_VALUE, "--gravitation-center-y", "-gcy",
                          "Gravitation center y coordinate [0.0]", "0.0", MK_OPTIONAL);
    a.add_argument_entry( "opacity", MK_VALUE, "--opacity", "-o",
                          "Opacity of edges", "0.3", MK_OPTIONAL);
    a.read_arguments(argc_, argv_);
    a.show_settings();

    // set params and read graph
    gGraph.set_algorithm_params( a.get_double_argument("K"),
                                 a.get_double_argument("I"),
                                 a.get_double_argument("compat"),
                                 a.get_double_argument("sigma"));
    gGraph.set_network_params( a.get_double_argument("weight threshold") );
    gGraph.set_physics_params( a.get_double_argument("S"),
                               a.get_double_argument("epsilon"),
                               a.get_double_argument("gravitation exponent") );
    gGraph.set_graphics_params( a.get_double_argument("opacity") );
    gMaxCycles = a.get_int_argument("cycles");
    gGraph.read(a.get_string_argument("nodes"),
                a.get_string_argument("edges"));

    // get dimensions
    meerkat::mk_vector2 bottomLeft, topRight;
    gGraph.get_bounding_box(bottomLeft, topRight, 20.0);
    double gWidth = topRight.x() - bottomLeft.x();
    double gHeight = topRight.y() - bottomLeft.y();

    // get window size
    int wWidth = DEFAULT_SIZE, wHeight = DEFAULT_SIZE;
    if( gWidth > gHeight )
        wHeight = int(wWidth * gHeight/gWidth);
    else
        wWidth = int(wHeight * gWidth/gHeight);

    glutInit( &argc_, argv_ );
    glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE );
    glutInitWindowSize( wWidth, wHeight );
    glutInitWindowPosition( 10, 100 );
    glutCreateWindow( "fdeb" );
    init(bottomLeft, topRight);
    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );
    glutTimerFunc( 100, timer, 1 );
    glutMainLoop();

    return 0;
}
