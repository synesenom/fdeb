#include "meerkat_vector2.hpp"
#include "math.h"

//////////////////////////////////
////////// CONSTRUCTORS //////////
//////////////////////////////////

/**
 * Desc: Empty constructor, creates a zero vector.
 */
meerkat::mk_vector2::mk_vector2()
{
  _x = 0.0;
  _y = 0.0;
}


/**
 * Desc: Constructor with the components.
 *
 * @x_ : x-component.
 * @y_ : y-component.
 */
meerkat::mk_vector2::mk_vector2( const double x_, const double y_ )
{
  _x = x_;
  _y = y_;
}



//////////////////////////////////////
////////// GETTERS, SETTERS //////////
//////////////////////////////////////
/**
 * Desc: Returns the x-component.
 *
 * return : x-component.
 */
double meerkat::mk_vector2::x() const
{
  return _x;
}


/**
 * Desc: Returns the y-component.
 *
 * return : y-component.
 */
double meerkat::mk_vector2::y() const
{
  return _y;
}


/**
 * Desc: Sets x-component.
 *
 * @x_ : x-component.
 */
void meerkat::mk_vector2::set_x( const double x_ )
{
  _x = x_;
}


/**
 * Desc: Sets y-component.
 *
 * @y_ : y-component.
 */
void meerkat::mk_vector2::set_y( const double y_ )
{
  _y = y_;
}


/**
 * Desc: Sets vector components.
 *
 * @x_ : new x-component.
 * @y_ : new y-component.
 */
void meerkat::mk_vector2::set( const double x_, const double y_ )
{
  _x = x_;
  _y = y_;
}



/////////////////////////////////////
////////// BASIC OPERATORS //////////
/////////////////////////////////////
/**
 * Desc: Assignment operator.
 *
 * @vector_ : vector to be equal to.
 * return : 
 */
meerkat::mk_vector2& meerkat::mk_vector2::operator=( const mk_vector2 &vector_ )
{
  _x = vector_._x;
  _y = vector_._y;

  return *this;
}


/**
 * Desc: Compound addition operator.
 *
 * @vector_ : addition vector.
 * return : increased vector.
 */
meerkat::mk_vector2& meerkat::mk_vector2::operator+=( const mk_vector2 &vector_ )
{
  _x += vector_._x;
  _y += vector_._y;

  return *this;
}


/**
 * Desc: Binary addition operator.
 *
 * @vector_ : addition vector.
 * return : increased vector.
 */
const meerkat::mk_vector2 meerkat::mk_vector2::operator+( const mk_vector2 &vector_ ) const
{
  mk_vector2 result = *this;
  result += vector_;

  return result;
}


/**
 * Desc: Compound subtraction operator.
 *
 * @vector_ : subtracted vector.
 * return : decreased vector.
 */
meerkat::mk_vector2& meerkat::mk_vector2::operator-=( const mk_vector2 &vector_ )
{
  _x -= vector_._x;
  _y -= vector_._y;

  return *this;
}


/**
 * Desc: Binary subtraction operator.
 *
 * @vector_ : subtracted vector.
 * return : decreased vector.
 */
const meerkat::mk_vector2 meerkat::mk_vector2::operator-( const mk_vector2 &vector_ ) const
{
  mk_vector2 result = *this;
  result -= vector_;

  return result;
}


/**
 * Desc: Scalar product.
 *
 * @vector_ : vector to multiply with.
 * return : scalar product of the vectors.
 */
const double meerkat::mk_vector2::operator*( const mk_vector2 &vector_ ) const
{
  return _x*vector_._x + _y*vector_._y;
}


/**
 * Compound scalar multiplication operator.
 *
 * @scalar_ : multiplier.
 * return : increased vector.
 */
meerkat::mk_vector2& meerkat::mk_vector2::operator*=( const double scalar_ )
{
  _x *= scalar_;
  _y *= scalar_;

  return *this;
}


/**
 * Desc: Binary subtraction operator.
 *
 * @vector_ : subtracted vector.
 * return : increased vector.
 */
const meerkat::mk_vector2 meerkat::mk_vector2::operator*( const double scalar_ ) const
{
  mk_vector2 result = *this;
  result *= scalar_;

  return result;
}


/**
 * Desc: Compound scalar division operator.
 *
 * @scalar_ : divider.
 * return : decreased vector.
 */
meerkat::mk_vector2& meerkat::mk_vector2::operator/=( const double scalar_ )
{
  _x /= scalar_;
  _y /= scalar_;

  return *this;
}


/**
 * Desc: Binary division operator.
 *
 * @vector_ : divider vector.
 * return : decreased vector.
 */
const meerkat::mk_vector2 meerkat::mk_vector2::operator/( const double scalar_ ) const
{
  mk_vector2 result = *this;
  result /= scalar_;

  return result;
}

//////////////////////////////////////
////////// OTHER OPERATIONS //////////
//////////////////////////////////////

/**
 * Desc: Returns the length of the vector.
 *
 * return : length of the vector.
 */
const double meerkat::mk_vector2::length() const
{
  return sqrt( _x*_x + _y*_y );
}


/**
 * Desc: Sets length of the vector.
 *
 * @length_ : new length.
 */
void meerkat::mk_vector2::set_length( const double length_ )
{
  double a = atan2( _y, _x );
  set( length_*cos(a), length_*sin(a) );
}


/**
 * Desc: Returns the argument of the vector.
 *
 * return : angle between the vector and the x-axis.
 */
const double meerkat::mk_vector2::angle() const
{
  return atan2( _y, _x );
}


/**
 * Desc: Sets argument of the vector.
 *
 * @angle_ : new argument.
 */
void meerkat::mk_vector2::set_angle( const double angle_ )
{
  double l = length();
  set( l*cos(angle_), l*sin(angle_) );
}


/**
 * Desc: Normalizes vector length to unit.
 */
void meerkat::mk_vector2::normalize()
{
  set_length( 1.0 );
}


/**
 * Desc: Rotates the vector by a given angle (in radian).
 *
 * @angle_ : angle of rotation in radian.
 */
void meerkat::mk_vector2::rotate( const double angle_ )
{
  double a = angle();
  set_angle( a + angle_ );  
}

