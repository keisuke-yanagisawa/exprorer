#include "Converter.hpp"
#include "utils.hpp"

#include <openbabel/mol.h>
#include <openbabel/bond.h>
#include <openbabel/obconversion.h>

#include <algorithm>
#include <string>
#include <cctype>
#include <cstdio>

namespace {
  using namespace std;

  std::string getAtomTypeName(const OpenBabel::OBAtom& oatom) {
    OpenBabel::OBAtom& atom = const_cast<OpenBabel::OBAtom&>(oatom);

    OpenBabel::ttab.SetFromType("INT");
    OpenBabel::ttab.SetToType("XYZ");
    
    if     (atom.IsCarbon()   &&  atom.IsAromatic())      return "A";
    else if(atom.IsCarbon()   && !atom.IsAromatic())      return "C";
    else if(atom.IsNitrogen() &&  atom.IsHbondAcceptor()) return "NA";
    else if(atom.IsNitrogen() && !atom.IsHbondAcceptor()) return "N";
    else if(atom.IsOxygen()   &&  atom.IsHbondAcceptor()) return "OA";
    else if(atom.IsOxygen()   && !atom.IsHbondAcceptor()) return "O";
    else if(atom.IsSulfur()   &&  atom.IsHbondAcceptor()) return "SA";
    else if(atom.IsSulfur()   && !atom.IsHbondAcceptor()) return "S";
    else if(atom.IsHydrogen() &&  atom.IsHbondDonorH())   return "HD";
    else if(atom.IsHydrogen() && !atom.IsHbondDonorH())   return "H";
    else {
      std::string src = atom.GetType();
      std::string dst;
      OpenBabel::ttab.Translate(dst, src);
      return dst;
    }
  }
  
}

namespace format {
  
  fragdock::Molecule Converter::toFragmentMol(const OpenBabel::OBMol &c_obmol){
    std::vector<fragdock::Atom> fatoms;
    OpenBabel::OBMol &obmol = const_cast<OpenBabel::OBMol&>(c_obmol);


    for(int i=0; i<obmol.NumAtoms(); i++) {
      OpenBabel::OBAtom& oatom = *( obmol.GetAtom(i+1) ); //obmol::index is 1 origin

      std::string atomtype = getAtomTypeName(oatom);
      fragdock::AtomType type(atomtype);
      fragdock::Vector3d pos(oatom.GetX(), oatom.GetY(), oatom.GetZ());
      fatoms.push_back( fragdock::Atom(oatom.GetId(), pos, type, oatom.GetPartialCharge()) );
    }
    fragdock::Molecule mol(fatoms);
    for(int i = 0; i < obmol.NumBonds(); i++){
      mol.append(fragdock::Bond(obmol.GetBond(i)->GetBeginAtom()->GetId(),
				obmol.GetBond(i)->GetEndAtom()->GetId(),
				obmol.GetBond(i)->IsRotor(false) ));
    }

    return mol;
  }

  OpenBabel::OBMol Converter::toOBMol(const fragdock::Molecule &mol, const OpenBabel::OBMol& original_obmol,
				      int capping_atomic_num = 6, bool capping_for_carbon = false) {
    using namespace fragdock;
    using namespace std;

    OpenBabel::OBMol obmol = original_obmol;
    obmol.SetTitle("");

    std::vector<int> del_id_list;
    for(int i=0; i<original_obmol.NumAtoms(); i++){
      del_id_list.push_back(i);
    }


    for(int i=0; i<mol.getAtoms().size(); i++) {
      int id = mol.getAtoms()[i].getId();

      del_id_list.erase(std::remove(del_id_list.begin(), del_id_list.end(), id), del_id_list.end()); 
      /*
      std::cout << mol.getAtoms()[i].getType().getName() << " " 
		<< original_obmol.GetAtomById(id)->GetType() << " "
		<< std::endl;
      */
    }

    if(capping_atomic_num >= 0){ // DO capping if non-negative value
      std::vector<int> include_list;
      for(int i=0; i<original_obmol.NumBonds(); i++){
	const OpenBabel::OBBond *bond = original_obmol.GetBond(i);
	int modification_id = -1;

	//ignore bond to hydrogen
	if(bond->GetBeginAtom()->GetAtomicNum() == 1 
	   or bond->GetEndAtom()->GetAtomicNum() == 1) continue;

	int atom_id1 = bond->GetBeginAtom()->GetId();
	int atom_id2 = bond->GetEndAtom()->GetId();

	bool id1_not_in_fragment = std::find(del_id_list.begin(),
					     del_id_list.end(),
					     atom_id1) != del_id_list.end();
	bool id2_not_in_fragment = std::find(del_id_list.begin(),
					     del_id_list.end(),
					     atom_id2) != del_id_list.end();
	
	if(id1_not_in_fragment == id2_not_in_fragment) continue;
	else if(id1_not_in_fragment){
	  if(!capping_for_carbon &&
	     bond->GetEndAtom()->GetAtomicNum() == 6) continue;
	  modification_id = atom_id1;
	}else{
	  if(!capping_for_carbon &&
	     bond->GetBeginAtom()->GetAtomicNum() == 6) continue;
	  modification_id = atom_id2;
	}
	include_list.push_back(modification_id);
	obmol.GetAtomById(modification_id)->SetAtomicNum(capping_atomic_num);
	obmol.GetAtomById(modification_id)->SetFormalCharge(0); // neutral
      }
      for(int i=0; i<include_list.size(); i++){
	del_id_list.erase(std::remove(del_id_list.begin(), del_id_list.end(), include_list[i]), del_id_list.end());
      }
    }

    for(int i=0; i<del_id_list.size(); i++){
      int id = del_id_list[i];
      //std::cout << "del list" << i << ": " << id << " : " << obmol.GetAtomById(id)->GetType() << std::endl;
      obmol.DeleteAtom(obmol.GetAtomById(id));
    }

    /*
    vector<Atom> atoms = mol.getAtoms();
    for(int i = 0; i < obmol.NumAtoms(); i++) {
      OpenBabel::OBAtom* atom = obmol.GetAtomById(i);
      double x = atoms[i].x;
      double y = atoms[i].y;
      double z = atoms[i].z;
      atom->SetVector(x, y, z);
    }
    */


    return obmol;
  }

}
