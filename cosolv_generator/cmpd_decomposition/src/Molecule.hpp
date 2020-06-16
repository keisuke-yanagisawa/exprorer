#include "common.hpp"
#include "Atom.hpp"
#include "Vector3d.hpp"
#include <map>
#include <iostream>
#ifndef MOLECULE_H_
#define MOLECULE_H_

namespace fragdock {
  struct Bond {
    int atom_id1, atom_id2;
    bool is_rotor;
    friend std::ostream& operator<< (std::ostream& os, const Bond& bond);
    Bond(const int atom_id1, const int atom_id2, const bool is_rotor) : atom_id1(atom_id1), atom_id2(atom_id2), is_rotor(is_rotor) {}
  };
  class Molecule {
  protected:
    std::vector<Atom> atoms;
    std::vector<Bond> bonds;
  public:
    Molecule() {}
    Molecule(const std::vector<Atom> &atoms) : atoms(atoms) {}
    void translate(const Vector3d &vec);
    void rotate(const Rotation& rot);
    void rotate(fltype theta, fltype phi, fltype psi);
    void rotate(double rmatrix[3][3]);
    void axisRotate(const Vector3d& axis, fltype th, const std::vector<int>& id_set);
    void axisRotate(const Vector3d& base, const Vector3d& axis, fltype th, const std::vector<int>& id_set);
    void bondRotate(const int bond_id, fltype th);
    void append(const Molecule &o);
    void append(const Atom &o);
    void append(const Bond &bond);
    Vector3d getCenter() const;
    int size() const { return atoms.size(); }
    const Atom& getAtom(int i) const { return atoms[i]; }
    const std::vector<Atom>& getAtoms() const { return atoms; }
    const std::vector<Bond>& getBonds() const { return bonds; }
    void sort();
    fltype calcRMSD(const Molecule& o) const;
    fltype superimpose(const Molecule& to);
    friend std::ostream& operator<< (std::ostream& os, const Molecule& mol);
  };
}
#endif
