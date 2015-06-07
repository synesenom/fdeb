#ifndef MEERKAT_VECTOR2_H
#define MEERKAT_VECTOR2_H

#include <vector>

namespace meerkat {

  class mk_vector2 {
  private:
    double _x;
    double _y;

  public:
    // Constructors
    mk_vector2();
    mk_vector2( const double x_, const double y_ );

    // Setters and getters
    double x() const;
    double y() const;
    void set_x( const double x_ );
    void set_y( const double y_ );
    void set( const double x_, const double y_ );

    // Basic operators
    // vector-vector
    mk_vector2& operator =( const mk_vector2 &vector_ );
    mk_vector2& operator +=( const mk_vector2 &vector_ );
    const mk_vector2 operator +( const mk_vector2 &vector_ ) const;
    mk_vector2& operator -=( const mk_vector2 &vector_ );
    const mk_vector2 operator -( const mk_vector2 &vector_ ) const;
    const double operator *( const mk_vector2 &vector_ ) const;
    // vector-scalar
    mk_vector2& operator *=( const double scalar_ );
    const mk_vector2 operator *( const double scalar_ ) const;
    mk_vector2& operator /=( const double scalar_ );
    const mk_vector2 operator /( const double scalar_ ) const;

    // Other operations
    const double length() const;
    const double angle() const;
    void set_length( const double length_ );
    void set_angle( const double angle_ );

    void normalize();
    void rotate( const double angle_ );
  };
}


#endif // MEERKAT_VECTOR2_H
