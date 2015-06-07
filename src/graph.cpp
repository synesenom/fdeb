#include "graph.hpp"

Graph::Graph()
{
    _log.tag("Graph");
    _K = 0.1;
    _S = 0.1;
    _I = 90;
    _C = 1;
    _compat = 0.6;
    _sigma = 0.2;
    _weightThreshold = 0.0;
    _epsilon = 1e-4;

    _alpha = 0.3;
}

void Graph::set_network_params(double weightThreshold_)
{
    _weightThreshold = weightThreshold_;
}

void Graph::set_algorithm_params(double K_, double I0_, double compat_, double sigma_)
{
    _K = K_;
    _I = I0_;
    _compat = compat_;
    _sigma = sigma_;
}

void Graph::set_physics_params(double S0_, double epsilon_, double beta_)
{
    _S = S0_;
    _epsilon = epsilon_;
    _beta = beta_;
}

void Graph::set_graphics_params(double alpha_)
{
    _alpha = alpha_;
}

int Graph::get_I()
{
    return _I;
}

int Graph::get_C()
{
    return _C;
}

void Graph::read(std::string nodesFile_, std::string edgesFile_)
{
    _log.i("read", "loading network");
    meerkat::mk_file_manager f;
    int rows = 0;
    char line[1024] = {""};

    // read nodes
    if( !f.open(nodesFile_, "r") )
    {
        _log.e("read", "could not read nodes file");
        exit(0);
    }
    rows = f.rows();
    f.get_text(line, 1024);
    char labelCh[128] = {""};
    double x, y;
    for( int r=1; r<rows; r++ )
    {
        f.get_text(line, 1024);
        sscanf(line, "%s %lg %lg", labelCh, &x, &y);
        _nodes.insert(std::pair<std::string, Node>(std::string(labelCh), Node(x, -y)));
    }
    _log.i("read", "number of nodes: %i", (int)_nodes.size());
    f.close();

    // read edges
    if( !f.open(edgesFile_, "r") )
    {
        _log.e("read", "could not read edges file");
        exit(0);
    }
    rows = f.rows();
    f.get_text(line, 1024);
    char src[128], dst[128];
    double w, wmax = 0.0;
    for( int r=0; r<rows-1; r++)
    {
        f.get_text(line, 1024);
        w = 1.0;
        sscanf(line, "%s %s %lg", src, dst, &w);
        if( w > _weightThreshold )
        {
            _edges.push_back(Edge());
            _edges.back()._start.set( _nodes[std::string(src)]._pos.x(), _nodes[std::string(src)]._pos.y() );
            _edges.back()._end.set( _nodes[std::string(dst)]._pos.x(), _nodes[std::string(dst)]._pos.y() );
            _edges.back()._width = w + 1.0;
            wmax = w > wmax ? w : wmax;
            _edges.back().arrange_direction();
            _edges.back().add_subdivisions();
        }
        _nodes[std::string(src)]._degree++;
        _nodes[std::string(dst)]._degree++;
    }
    int edgesNum = (int)_edges.size();
    for( int i=0; i<edgesNum; i++ )
        _edges[i]._width *= 1.0 / (wmax+1.0);
    _log.i("read", "number of edges: %i", (int)_edges.size());
    f.close();

    // build compability lists
    build_compatibility_lists();
}

void Graph::get_bounding_box(meerkat::mk_vector2 &bottomLeft_,
                             meerkat::mk_vector2 &topRight_,
                             double frame_)
{
    std::map<std::string, Node>::iterator it = _nodes.begin();
    bottomLeft_ = it->second._pos;
    topRight_ = it->second._pos;
    it++;
    double x, y;
    for( ;
         it != _nodes.end();
         it++ )
    {
        x = it->second._pos.x();
        y = it->second._pos.y();
        if( x < bottomLeft_.x() )
            bottomLeft_.set_x(x);
        if( y < bottomLeft_.y() )
            bottomLeft_.set_y(y);
        if( x > topRight_.x() )
            topRight_.set_x(x);
        if( y > topRight_.y() )
            topRight_.set_y(y);
    }
    bottomLeft_.set(bottomLeft_.x()-frame_, bottomLeft_.y()-frame_);
    topRight_.set(topRight_.x()+frame_, topRight_.y()+frame_);
    _log.i("get_bounding_box", "bounding box: (%lg, %lg, %lg, %lg)",
           bottomLeft_.x(), bottomLeft_.y(), topRight_.x(), topRight_.y());
}

void Graph::build_compatibility_lists()
{
    _log.i("build_compability_lists", "building lists");

    int edgesNum = (int)_edges.size(), compEdgePairs = 0;
    double comp = 0.0;
    for( int i=0; i<edgesNum; i++ )
    {
        for( int j=i+1; j<edgesNum; j++ )
        {
            comp = angle_compatilibity(_edges[i], _edges[j])
                    * scale_compatibility(_edges[i], _edges[j])
                    * position_compatibility(_edges[i], _edges[j])
                    * visibility_compability(_edges[i], _edges[j]);
            if( comp >= _compat )
            {
                _edges[i]._compatibleEdges.push_back(j);
                _edges[i]._compatibilities.push_back(comp);
                _edges[j]._compatibleEdges.push_back(i);
                _edges[j]._compatibilities.push_back(comp);
                compEdgePairs++;
            }
        }
    }
    _log.i("build_compatibility_lists", "compatible edge pairs: %i", compEdgePairs);
}

void Graph::iterate()
{
    int edgesNum = (int)_edges.size();
    std::vector<std::vector<meerkat::mk_vector2> > forces(
                edgesNum,
                std::vector<meerkat::mk_vector2>(
                    (int)_edges[0]._subdivs.size(), meerkat::mk_vector2(0.0, 0.0))
            );

    // spring forces
    for( int i=0; i<edgesNum; i++ )
        _edges[i].add_spring_forces(forces[i], _K);

    // electrostatic forces
    for( int i=0; i<edgesNum; i++ )
    {
        int compatibleEdgesNum = (int)_edges[i]._compatibleEdges.size();
        for( int j=0; j<compatibleEdgesNum; j++ )
            _edges[i].add_electrostatic_forces(forces[i],
                                               _edges[_edges[i]._compatibleEdges[j]],
                                               _epsilon);
    }

    // gravitation
    /*meerkat::mk_vector2 center(0.0, 0.0);
    for( int i=0; i<edgesNum; i++ )
        _edges[i].add_gravitational_forces(forces[i], center, _beta);
*/
    // update edges
    for( int i=0; i<edgesNum; i++ )
        _edges[i].update(forces[i], _S);
}

void Graph::update_cycle()
{
    _log.i("update_cycle", "updating parameters");
    _S *= 0.5;
    _I = 2*_I/3;
    _C++;

    _log.i("update_cycle", "subdividing edges");
    int edgesNum = (int)_edges.size();
    for( int i=0; i<edgesNum; i++ )
        _edges[i].add_subdivisions();
}

void Graph::smooth()
{
    _log.i("smooth", "applying Gaussian smoothing");
    int edgesNum = (int)_edges.size();
    for( int i=0; i<edgesNum; i++ )
        _edges[i].smooth(_sigma);
    _log.i("smooth", "done");
}

void Graph::draw()
{
    // draw edges
    int numEdges = (int)_edges.size();
    for( int i=0; i<numEdges; i++ )
        _edges[i].draw(_alpha);

    // draw nodes
    for( std::map<std::string, Node>::iterator it = _nodes.begin();
         it != _nodes.end();
         it++ )
        it->second.draw();
}
