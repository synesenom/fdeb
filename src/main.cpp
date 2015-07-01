#include "stdlib.h"
#include "graph.hpp"
#include "GLUT/glut.h"
#include "meerkat_argument_manager.hpp"
#include "meerkat_vector2.hpp"

#define DEFAULT_SIZE 600

Graph gGraph;
std::string gJson = "";

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
    if( gGraph.iterate() > 0 )
        glutTimerFunc( 10, timer, var_ );
    else
    {
        if( gGraph.update_cycle() > 0 )
        {
            gGraph.add_subvisions();
            glutTimerFunc( 10, timer, var_ );
        }
        else
        {
            gGraph.smooth();
            if( gJson != "" )
                gGraph.print_json(gJson);
        }
    }
    display();
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
    a.add_argument_entry( "json", MK_VALUE, "--json", "-json",
                          "Prints result in a JSON file [unset]", "", MK_OPTIONAL);
    a.add_argument_entry( "K", MK_VALUE, "--K", "-K",
                          "Edge stiffness [0.1]", "0.1", MK_OPTIONAL);
    a.add_argument_entry( "S", MK_VALUE, "--S", "-S",
                          "Initial value of S [0.4]", "0.4", MK_OPTIONAL);
    a.add_argument_entry( "I", MK_VALUE, "--I", "-i",
                          "Initial number of iterations [90]", "90", MK_OPTIONAL);
    a.add_argument_entry( "compat", MK_VALUE, "--compat", "-c",
                          "Compatibility threshold [0.6]", "0.6", MK_OPTIONAL);
    a.add_argument_entry( "cycles", MK_VALUE, "--cycles", "-C",
                          "Max number of cycles [5]", "5", MK_OPTIONAL);
    a.add_argument_entry( "sigma", MK_VALUE, "--sigma", "-s",
                          "Smoothing width [3]", "3", MK_OPTIONAL);
    a.add_argument_entry( "edge weight", MK_VALUE, "--edge-weight", "-ew",
                          "Edge weight threshold for edge filtering [unset]."
                          "Edges with a weight below the given value are dropped",
                          "-1.0", MK_OPTIONAL);
    a.add_argument_entry( "edge percentage", MK_VALUE, "--edge-percentage", "-ep",
                          "Edge percentage for edge filtering [unset]."
                          "The given fraction of edges among the strongest ones are kept",
                          "-1.0", MK_OPTIONAL);
    a.add_argument_entry( "epsilon", MK_VALUE, "--epsilon", "-E",
                          "Lowest interaction distance [1e-4]", "0.0001", MK_OPTIONAL);
    a.add_argument_entry( "gravitation center x", MK_VALUE, "--gravitation-center-x", "-gcx",
                          "Gravitation center x coordinate [0.0]. If set, gravitation is turned on",
                          "0.0", MK_OPTIONAL);
    a.add_argument_entry( "gravitation center y", MK_VALUE, "--gravitation-center-y", "-gcy",
                          "Gravitation center y coordinate [0.0]. If set, gravitation is turned on",
                          "0.0", MK_OPTIONAL);
    a.add_argument_entry( "gravitation exponent", MK_VALUE, "--gravitation-exponent", "-ge",
                          "Gravitation exponent [-2.0]. If set, gravitation is turned on",
                          "1.0", MK_OPTIONAL);
    a.add_argument_entry( "visualization", MK_FLAG, "--visualize", "-v",
                          "Enables real-time visualization [off]", "0", MK_OPTIONAL);
    a.add_argument_entry( "transparency", MK_VALUE, "--transparency", "-t",
                          "Transparency of edges", "0.3", MK_OPTIONAL);
    a.read_arguments(argc_, argv_);
    a.show_settings();

    // set params and read graph
    gGraph.set_algorithm_params( a.get_double_argument("K"),
                                 a.get_int_argument("cycles"),
                                 a.get_int_argument("I"),
                                 a.get_double_argument("compat"),
                                 a.get_double_argument("sigma"));
    gGraph.set_network_params( a.get_double_argument("edge weight"),
                               a.get_double_argument("edge percentage") );
    gGraph.set_physics_params( a.get_double_argument("S"),
                               a.get_double_argument("epsilon"),
                               meerkat::mk_vector2(a.get_double_argument("gravitation center x"),
                                                   a.get_double_argument("gravitation center y")),
                               a.get_double_argument("gravitation exponent") );
    if( a.is_set("gravitation center x") ||
            a.is_set("gravitation center y") ||
            a.is_set("gravitation exponent") )
        gGraph.enable_gravitation();

    // Read graph
    gGraph.read(a.get_string_argument("nodes"),
                a.get_string_argument("edges"));

    // Get output name
    if( a.is_set("json") )
        gJson = a.get_string_argument("json");

    // If visualization is enabled
    if( a.is_set("visualization") )
    {
        // Set graphical parameters
        gGraph.set_graphics_params( a.get_double_argument("transparency") );

        // Get bounding box
        meerkat::mk_vector2 bottomLeft, topRight;
        gGraph.get_bounding_box(bottomLeft, topRight, 20.0);
        double gWidth = topRight.x() - bottomLeft.x();
        double gHeight = topRight.y() - bottomLeft.y();

        // Init GLUT
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

        // Increase cycle index and enter main loop
        glutDisplayFunc( display );
        glutKeyboardFunc( keyboard );
        glutTimerFunc( 10, timer, 1 );
        glutMainLoop();
    }
    // Otherwise, just perform edge bundling
    else
    {
        do
        {
            while( gGraph.iterate() > 0 );
            gGraph.add_subvisions();
        } while( gGraph.update_cycle() > 0 );
        gGraph.smooth();
        if( gJson != "" )
            gGraph.print_json(gJson);
    }

    return 0;
}
