#include "common.hpp"
#include <cmath>
#include <iostream>
#ifndef VECTOR3D_H_
#define VECTOR3D_H_

namespace fragdock {
  struct Rotation;
  class Vector3d {
  public:
    fltype x, y, z;
    Vector3d() { x = y = z = 0.0; }
    Vector3d(fltype x, fltype y, fltype z) : x(x), y(y), z(z) {}
  
    fltype dot(const Vector3d &o) const { return x*o.x + y*o.y + z*o.z; }
    Vector3d cross(const Vector3d &o) const { return Vector3d(y*o.z - z*o.y, z*o.x - x*o.z, x*o.y - y*o.x); }
    fltype abs() const { return sqrt(norm()); }
    fltype norm() const { return dot(*this); }
    void axisRotate(const Vector3d &axis, double th);
    Vector3d unit() { fltype ab = this->abs(); return Vector3d(x/ab, y/ab, z/ab); }
    fltype getAngle(const Vector3d &o) const { return acos(dot(o)/sqrt(this->norm()*o.norm())); }
    void rotate(const Rotation& rot);
    void rotate(fltype theta, fltype phi, fltype psi);
    void translate(const Vector3d &o) { x += o.x, y += o.y, z += o.z; }
    Vector3d operator-(const Vector3d &o) const { return Vector3d(x-o.x, y-o.y, z-o.z); }
    Vector3d operator-=(const Vector3d &o) { x -= o.x; y -= o.y; z -= o.z; return *this; }
    Vector3d operator+(const Vector3d &o) const { return Vector3d(x+o.x, y+o.y, z+o.z); }
    Vector3d operator+=(const Vector3d &o) { x += o.x; y += o.y; z += o.z; return *this; }
    Vector3d operator*(const fltype o) const { return Vector3d(x*o, y*o, z*o); }
    Vector3d operator*=(const fltype o) { x *= o, y *= o, z *= o; return Vector3d(x*o, y*o, z*o); }
    Vector3d operator/(const fltype o) const { return *this*(1.0/o); }
    Vector3d operator/=(const fltype o) { x /= o, y /= o, z /= o; return *this; }
    Vector3d operator-() const { return Vector3d(-x, -y, -z); }
    void print() const { std::cout << "(" << x << ", " << y << ", " << z << ")" << std::endl; }
  };

  struct Rotation {
    Vector3d dir;
    fltype th;
    Rotation() : dir(Vector3d(0.0, 0.0, 0.0)), th(0.0) {}
    Rotation(fltype x, fltype y, fltype z, fltype th) : dir(Vector3d(x, y, z)), th(th) {
      if(abs(x*x + y*y + z*z - 1.0) > 1e-6) std::cerr << "rotation dir is not unit vector" << std::endl;
    }
    Rotation(const Vector3d& dir, fltype th) : dir(dir), th(th) {
      if(abs(dir.x*dir.x + dir.y*dir.y + dir.z*dir.z - 1.0) > 1e-6) std::cerr << "rotation dir is not unit vector" << std::endl;
    }
    static std::vector<fragdock::Rotation> makeRotations();
    static std::vector<fragdock::Rotation> makeRotationsRandom();    
  };

}
#endif
