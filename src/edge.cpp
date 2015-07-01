#include "edge.hpp"


Edge::Edge(std::string sourceLabel_, std::string targetLabel_,
           meerkat::mk_vector2 &start_, meerkat::mk_vector2 &end_,
           double width_)
{
    _sourceLabel = sourceLabel_;
    _targetLabel = targetLabel_;
    _start = start_;
    _end = end_;
    _width = width_;
    arrange_direction();
    add_subdivisions();
}

meerkat::mk_vector2 Edge::center(meerkat::mk_vector2 &p1_, meerkat::mk_vector2 &p2_)
{
    return (p1_+p2_)/2.0;
}

meerkat::mk_vector2 Edge::project(meerkat::mk_vector2 &point_,
                                  meerkat::mk_vector2 &lineStart_,
                                  meerkat::mk_vector2 &lineEnd_)
{
    double L = (lineStart_-lineEnd_).length();
    double r = ((lineStart_.y()-point_.y())*(lineStart_.y()-lineEnd_.y())
                - (lineStart_.x()-point_.x())*(lineEnd_.x()-lineStart_.x())) / (L * L);
    return lineStart_ + (lineEnd_-lineStart_) * r;
}

double gauss_weight(int dist_, double sigma_)
{
    return INV_SQRT_2PI * exp(-0.5*pow(double(dist_)/sigma_, 2.0)) / sigma_;
}

meerkat::mk_vector2 Edge::vector()
{
    return _end-_start;
}

void Edge::add_subdivisions()
{
    int oldSubdivsNum = (int)_subdivs.size();
    if( oldSubdivsNum == 0 )
        _subdivs.assign( 1, center(_start, _end) );
    else
    {
        int newSubdivsNum = 2 * oldSubdivsNum, subdivIndex = 0, v1Index = -1, v2Index = 0;
        double segmentLength = double(oldSubdivsNum+1)/double(newSubdivsNum+1);
        std::vector<meerkat::mk_vector2> subdivisions(newSubdivsNum, meerkat::mk_vector2());
        meerkat::mk_vector2 v1 = _start, v2 = _subdivs[0];
        double r = segmentLength;
        while( subdivIndex < newSubdivsNum )
        {
            subdivisions[subdivIndex] = v1 + (v2 - v1) * r;
            subdivIndex++;
            if( r + segmentLength > 1.0 )
            {
                r = segmentLength - (1.0 - r);
                v1Index++;
                v2Index++;

                if( v1Index >= 0 )
                    v1 = _subdivs[v1Index];
                if( v2Index < oldSubdivsNum )
                    v2 = _subdivs[v2Index];
                else
                    v2 = _end;
            }
            else
                r += segmentLength;
        }
        _subdivs = subdivisions;
    }
}

void Edge::arrange_direction()
{
    meerkat::mk_vector2 v = vector(), t;
    if( (fabs(v.x()) > fabs(v.y()) && _end.x() < _start.x())
            || (fabs(v.x()) < fabs(v.y()) && _end.y() < _start.y()) )
    {
        t = _start;
        _start = _end;
        _end = t;
    }
}

void Edge::add_spring_forces(std::vector<meerkat::mk_vector2> &forces_, double K_)
{
    int len = (int)_subdivs.size();
    double kP = K_ / ((_end-_start).length() * double(len+1));

    if( len == 1 )
        forces_[0] += (_start+_end-_subdivs[0]*2.0) * kP;
    else
    {
        // first division point
        forces_[0] += (_start+_subdivs[1]-_subdivs[0]*2.0) * kP;
        // inner division points
        for( int i=1; i<len-1; i++ )
            forces_[i] += (_subdivs[i-1]+_subdivs[i+1]-_subdivs[i]*2.0) * kP;
        // last division point
        forces_[len-1] += (_subdivs[len-2]+_end-_subdivs[len-1]*2.0) * kP;
    }
}

void Edge::add_electrostatic_forces(std::vector<meerkat::mk_vector2> &forces_,
                                    Edge edge_, double epsilon_)
{
    int len = (int)_subdivs.size();
    meerkat::mk_vector2 dist;
    double dlen;

    for( int i=0; i<len; i++ )
    {
        dist = (edge_._subdivs[i]-_subdivs[i]);
        dlen = dist.length();
        if( dlen > epsilon_ )
            forces_[i] += dist / dlen;
    }
}

void Edge::add_gravitational_forces(std::vector<meerkat::mk_vector2> &forces_,
                                    meerkat::mk_vector2 &center_,
                                    double exponent_)
{
    int len = (int)_subdivs.size();
    meerkat::mk_vector2 dist;
    double dlen;

    for( int i=0; i<len; i++ )
    {
        dist = (center_ - _subdivs[i]);
        dlen = dist.length();
        forces_[i] += dist * 0.1 * pow(dlen+1.0, exponent_);
    }
}

void Edge::update(std::vector<meerkat::mk_vector2> &forces_, double S_)
{
    int len = (int)_subdivs.size();
    double flen = 0.0;
    for( int i=0; i<len; i++ )
    {
        flen = forces_[i].length();
        if( flen > EPSILON )
            _subdivs[i] += forces_[i] * S_ / flen;
    }
}

void Edge::smooth(double sigma_)
{
    int len = (int)_subdivs.size();
    std::vector<meerkat::mk_vector2> subdivisions(len, meerkat::mk_vector2(0.0, 0.0));
    double weight, total_weight;
    for( int i=0; i<len; i++ )
    {
        total_weight = 0.0;

        // start point
        weight = gauss_weight(i+1, sigma_);
        subdivisions[i] += _start * weight;
        total_weight += weight;
        // inner points
        for( int j=0; j<len; j++ )
        {
            weight = gauss_weight(i-j, sigma_);
            subdivisions[i] += _subdivs[j] * weight;
            total_weight += weight;
        }
        // end point
        weight = gauss_weight(len-i+1, sigma_);
        subdivisions[i] += _end * weight;
        total_weight += weight;

        // normalize
        subdivisions[i] /= total_weight;
    }

    // replace division points
    _subdivs = subdivisions;
}

void Edge::draw(double alpha_)
{
    glLineWidth( _width );
    glColor4f( 212./255., 0./255., 0./255., alpha_ );
    int len = (int)_subdivs.size();

    glBegin( GL_LINES );
    // first segment
    glVertex2d( _start.x(), _start.y() );
    glVertex2d( _subdivs[0].x(), _subdivs[0].y() );
    // inner segments
    for( int i=0; i<len-1; i++ )
    {
        glVertex2d( _subdivs[i].x(), _subdivs[i].y() );
        glVertex2d( _subdivs[i+1].x(), _subdivs[i+1].y() );
    }
    // last segment
    glVertex2d( _subdivs[len-1].x(), _subdivs[len-1].y() );
    glVertex2d( _end.x(), _end.y() );
    glEnd();
}

double Edge::edge_visibility(Edge &edge1_, Edge &edge2_)
{
    meerkat::mk_vector2 I0 = project(edge1_._start, edge2_._start, edge2_._end);
    meerkat::mk_vector2 I1 = project(edge1_._end, edge2_._start, edge2_._end);
    meerkat::mk_vector2 midI = center(I0, I1);
    meerkat::mk_vector2 midP = center(edge2_._start, edge2_._end);
    return std::max(0.0, 1.0-2.0*(midP-midI).length()/(I0-I1).length());
}

double Edge::angle_compatilibity(Edge &edge1_, Edge &edge2_)
{
    meerkat::mk_vector2 v1 = edge1_.vector();
    meerkat::mk_vector2 v2 = edge2_.vector();
    v1.normalize();
    v2.normalize();
    return fabs(v1 * v2);;
}

double Edge::scale_compatibility(Edge &edge1_, Edge &edge2_)
{
    double l1 = edge1_.vector().length();
    double l2 = edge2_.vector().length();
    double lavg = (l1 + l2) / 2.0;
    if( lavg > EPSILON )
        return 2.0 / (lavg/std::min(l1, l2) + std::max(l1, l2)/lavg);
    else
        return 0.0;
}

double Edge::position_compatibility(Edge &edge1_, Edge &edge2_)
{
    double lavg = (edge1_.vector().length()+edge2_.vector().length()) / 2.0;
    if( lavg > EPSILON )
    {
        meerkat::mk_vector2 mid1 = center(edge1_._start, edge1_._end);
        meerkat::mk_vector2 mid2 = center(edge2_._start, edge2_._end);
        return lavg / (lavg + (mid1-mid2).length());
    }
    else
        return 0.0;
}

double Edge::visibility_compability(Edge &edge1_, Edge &edge2_)
{
    return std::min(edge_visibility(edge1_, edge2_),
                    edge_visibility(edge2_, edge1_));
}

int compare_edges( Edge edge1_,  Edge edge2_ )
{
    if( edge1_._width < edge2_._width )
        return 1;
    else if( edge1_._width > edge2_._width )
        return -1;
    else
        return 0;
}

