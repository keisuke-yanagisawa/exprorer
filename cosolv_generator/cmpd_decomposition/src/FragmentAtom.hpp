#include "common.hpp"
#ifndef FRAGMENT_ATOM_H_
#define FRAGMENT_ATOM_H_

class FragmentAtom : Vector3d {
  AtomType type;
  fltype  radius;
  void setRadius();
public:
  FragmentAtom(fltype x, fltype y, fltype z, AtomType type) : x(x), y(y), z(z), type(type) {
    setRadius();
  }
  AtomType getType() { return type; }
  fltype getRadius() { return radius; }
};

#endif
