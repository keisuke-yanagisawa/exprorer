#include <openbabel/mol.h>
#include "Molecule.hpp"
#include "UnionFindTree.hpp"

namespace fragdock {
  std::ostream& operator<< (std::ostream& os, const Bond& bond) {
    os << "From:" << bond.atom_id1 << " To:" << bond.atom_id2;
    return os;
  }


  void Molecule::translate(const Vector3d &vec) {
    for(int i = 0; i < atoms.size(); i++) 
      atoms[i].translate(vec);
  }

  void Molecule::rotate(const Rotation &rot) {
    translate(-getCenter());
    for(int i = 0; i < atoms.size(); i++)
      atoms[i].rotate(rot);
  }

  void Molecule::rotate(fltype theta, fltype phi, fltype psi) {
    translate(-getCenter());
    for(int i = 0; i < atoms.size(); i++)
      atoms[i].rotate(theta, phi, psi);
  }

  void Molecule::rotate(double rmatrix[3][3]) {
    for(int i=0; i<atoms.size(); i++){
      double x = atoms[i].x;
      double y = atoms[i].y;
      double z = atoms[i].z;
      atoms[i].x = rmatrix[0][0]*x + rmatrix[0][1]*y + rmatrix[0][2]*z;
      atoms[i].y = rmatrix[1][0]*x + rmatrix[1][1]*y + rmatrix[1][2]*z;
      atoms[i].z = rmatrix[2][0]*x + rmatrix[2][1]*y + rmatrix[2][2]*z;
    }
  }

  void Molecule::axisRotate(const Vector3d &axis, fltype th, const std::vector<int>& id_set) {
    for(int i = 0; i < atoms.size(); i++)
      atoms[i].axisRotate(axis, th);
  }

  void Molecule::axisRotate(const Vector3d& base, const Vector3d& axis, fltype th, const std::vector<int>& id_set) {
    Vector3d pos = getCenter();
    translate(-base);
    for(int i = 0; i < id_set.size(); i++)
      atoms[id_set[i]].axisRotate(axis, th);
    translate(pos - getCenter());
  }

  void Molecule::bondRotate(const int bond_id, fltype th){
    if(bond_id >= bonds.size()){
      std::cerr << "[Molecule::bondRotate] invarid bond id: " << bond_id << std::endl;
      exit(1);
    }

    // detect the bond is in a ring or not 
    utils::UnionFindTree uf((int)atoms.size());
    for(int i=0; i<bonds.size(); i++){
      if (i == bond_id) continue;
      const Bond &bond = bonds[i];
      uf.unite(bond.atom_id1, bond.atom_id2);
    }
    if(uf.getSets()[0].size() == (int)atoms.size()) return; // do nothing
    fragdock::Vector3d bond_axis = atoms[bonds[bond_id].atom_id2] - atoms[bonds[bond_id].atom_id1];
    axisRotate(atoms[bonds[bond_id].atom_id1], bond_axis, th, uf.getSets()[0]);
    //std::cout << *this << std::endl;
  }


  void Molecule::append(const Molecule &o) {
    for(int i = 0; i < o.size(); i++)
      append(o.getAtom(i));
    for(int i = 0; i < o.getBonds().size(); i++){
      append(o.getBonds()[i]);
    }
  }

  void Molecule::append(const Atom &o) {
    atoms.push_back(o);
  }

  void Molecule::append(const Bond &bond) {
    bonds.push_back(bond);
  }

  Vector3d Molecule::getCenter() const {
    Vector3d center(0.0, 0.0, 0.0);
    for(int i = 0; i < atoms.size(); i++)
      center += atoms[i];
    return center/(fltype)atoms.size();
  }
  void Molecule::sort() {
    std::sort(atoms.begin(), atoms.end());    
  }
  fltype Molecule::calcRMSD(const Molecule& o) const {
    using namespace std;
    if(this->size() != o.size()) {
      cerr << "calcRMSD: atom size is differ!!" << endl;
    }
    fltype sum = 0.0;
    for(int i = 0; i < this->size(); i++) {
      sum += (this->getAtom(i) - o.getAtom(i)).norm();
    }
    sum /= this->size();
    return sqrt(sum);
  }
  
  fltype Molecule::superimpose(const Molecule& ref) {
    //std::cout << "[debug] Molecule::superimpose start" << std::endl;
    if(this->size() != ref.size()){
      std::cerr << "superimpose: atom size is differ !!" << std::endl;
      exit(1);
    }
    Molecule temp_ref = ref;
    temp_ref.translate(-temp_ref.getCenter());
    this->translate(-this->getCenter());
    
    // data conversion
    double ref_coords[ref.size()*3];
    double mol_coords[this->size()*3];
    for(int i=0; i<this->size(); i++){
      ref_coords[3*i  ] = temp_ref.getAtom(i).x;
      ref_coords[3*i+1] = temp_ref.getAtom(i).y;
      ref_coords[3*i+2] = temp_ref.getAtom(i).z;
      mol_coords[3*i  ] = this->getAtom(i).x;
      mol_coords[3*i+1] = this->getAtom(i).y;
      mol_coords[3*i+2] = this->getAtom(i).z;
    }
    
    double rmatrix[3][3] = {};
    OpenBabel::qtrfit(ref_coords, mol_coords, this->size(), rmatrix);
    this->rotate(rmatrix);
    this->translate(ref.getCenter());
    return this->calcRMSD(ref);
  }
  std::ostream& operator<< (std::ostream& os, const Molecule& mol){
    std::vector<Atom> atoms = mol.atoms;
    std::vector<Bond> bonds = mol.bonds;

    for(int i=0; i<atoms.size(); i++){
      Atom atom = atoms[i];
      os << atom << std::endl;
    }

    for(int i=0; i<bonds.size(); i++){
      Bond bond = bonds[i];
      os << "BondId:" << i << " " << bond << std::endl;
    }
    return os;
  }
}
