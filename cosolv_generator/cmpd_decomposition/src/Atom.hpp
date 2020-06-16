#include "common.hpp"
#include "AtomType.hpp"
#include "Vector3d.hpp"
#ifndef ATOM_H_
#define ATOM_H_
namespace fragdock {
  class Atom : public Vector3d {
    int atom_id;
    AtomType type;
    fltype charge;
  public:
    Atom(int atom_id, const Vector3d &pos, const AtomType &type, fltype charge) : atom_id(atom_id), type(type), charge(charge){
      this->x = pos.x; this->y = pos.y; this->z = pos.z;
    }
    int getId() const { return atom_id; }
    AtomType getType() const { return type; }
    fltype getCharge() const { return charge; }
    bool operator<(const Atom& o) const { return atom_id < o.atom_id; }
    friend std::ostream& operator<< (std::ostream& os, const Atom& atom);
  };
}
#endif
