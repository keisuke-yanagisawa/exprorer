#include <algorithm>

#include "MoleculeToFragments.hpp"
#include "UnionFindTree.hpp"
#include "utils.hpp"
namespace {
  using namespace std;
  using namespace fragdock;

  vector<vector<int> > edges;
  vector<int> ring;
  vector<int> route;
  vector<bool> done;
  
  void dfs(int now, int start) {
    if(done[now]) {
      if(now == start and route.size() > 2) {
	ring = route;
      }
      return;
    }
    done[now] = true;
    
    route.push_back(now);
    for(int i = 0; i < edges[now].size(); i++)
      dfs(edges[now][i], start);
    
    route.pop_back();
  }
  
  vector<vector<int> > ringDetector(int atom_num, const vector<Bond> &bonds) {
    edges = vector<vector<int> >(atom_num, vector<int>());
    for(int i = 0; i < bonds.size(); i++) {
      const Bond &bond = bonds[i];
      int a = bond.atom_id1;
      int b = bond.atom_id2;
      edges[a].push_back(b);
      edges[b].push_back(a);
    }
    
    vector<vector<int> > ret;
    for(int i = 0; i < atom_num; i++) {
      done = vector<bool>(atom_num, false);
      ring = vector<int>();
      route = vector<int>();
      dfs(i, i);
      if(ring.size() > 0) ret.push_back(ring);
    }
    
    return ret;
  }

  template<typename Container>
  bool exist_in(const Container& c, const typename Container::value_type& v){
    return ( c.end() != std::find(c.begin(),c.end(),v) );
  }  

  Molecule gen_new_mol(const Molecule& mol, const std::vector<int>& id_map) {
    const vector<Atom> &atoms = mol.getAtoms();
    const vector<Bond> &bonds = mol.getBonds();

    Molecule temp_mol;
    for(int j=0; j<id_map.size(); j++){
      temp_mol.append(atoms[id_map[j]]);
    }
    for(int j=0; j<bonds.size(); j++){
      if(exist_in(id_map, bonds[j].atom_id1) && exist_in(id_map, bonds[j].atom_id2)){
	Bond new_bond = bonds[j];
	new_bond.atom_id1 = std::find(id_map.begin(), id_map.end(), bonds[j].atom_id1) - id_map.begin();
	new_bond.atom_id2 = std::find(id_map.begin(), id_map.end(), bonds[j].atom_id2) - id_map.begin();
	temp_mol.append(new_bond);
      }
    }
    return temp_mol;
  }

}

namespace fragdock {
  vector<Fragment> getFragments(const Molecule &mol) {
    const vector<Atom> &atoms = mol.getAtoms();
    const vector<Bond> &bonds = mol.getBonds();
    // unite two atoms if the bond between them is NOT 'Single'
    utils::UnionFindTree uf((int)atoms.size());
    for(int i = 0; i < bonds.size(); i++) {
      const Bond &bond = bonds[i];
      int a = bond.atom_id1;
      int b = bond.atom_id2;
      if(!bond.is_rotor 
	 && atoms[bond.atom_id1].getType().getName().find("H") != 0
	 && atoms[bond.atom_id2].getType().getName().find("H") != 0)
	uf.unite(a, b);      
    }

    // test internal rotation invariant
    for(int i=0; i<bonds.size(); i++){
      //std::cout << "test" << i << " " << bonds[i] << std::endl;
      // generate connected molecule
      const Bond &bond = bonds[i];
      int a = bond.atom_id1;
      int b = bond.atom_id2;
      if(atoms[a].getType().getName().find("H") == 0
	 || atoms[b].getType().getName().find("H") == 0) continue;
      if(uf.same(a,b)) continue; // no longer needed to do anything
      utils::UnionFindTree uf_temp(uf);
      uf_temp.unite(a, b);
      const std::vector<std::vector<int> > prev_sets = uf.getSets();
      const std::vector<std::vector<int> > united_sets = uf_temp.getSets();
      std::vector<int> united_set;
      std::vector<int> prev_set_a, prev_set_b;
      for(int i=0; i<united_sets.size(); i++){
	if(exist_in(united_sets[i], a)){
	  united_set = united_sets[i];
	  break;
	}
      }
      for(int i=0; i<prev_sets.size(); i++){
	if(exist_in(prev_sets[i], a)){
	  prev_set_a = prev_sets[i];
	}
	if(exist_in(prev_sets[i], b)){
	  prev_set_b = prev_sets[i];
	}
      }

      Molecule united_mol = gen_new_mol(mol, united_set);
      Molecule prev_mol_a = gen_new_mol(mol, prev_set_a);
      Molecule prev_mol_b = gen_new_mol(mol, prev_set_b);
      int nRings_prev = ringDetector(prev_mol_a.size(), prev_mol_a.getBonds()).size()
	+ ringDetector(prev_mol_b.size(), prev_mol_b.getBonds()).size();
      int nRings_united = ringDetector(united_mol.size(), united_mol.getBonds()).size();

      // avoid new ring generation
      if(nRings_prev != nRings_united){ // there are new rings
	continue;
      }

      bool ok = true; // is it ok to merge?
      // internal rotation test
      for(int j=0; j<united_mol.getBonds().size(); j++){
	Molecule test_united_mol;
	test_united_mol.append(united_mol);
	if(united_mol.getBonds()[j].is_rotor != true) continue;
	test_united_mol.bondRotate(j, 1); //1 rad rotation
	//std::cout << test_united_mol;
	//std::cout << united_mol.size() << " " << united_mol.superimpose(test_united_mol) << std::endl;
	if(united_mol.superimpose(test_united_mol) >= 1e-5){
	  ok = false;
	  break;
	}
      }
      if(ok){
	//std::cout << "unite" << std::endl;
	uf.unite(a, b);
      }
    }

    // unite H
    for(int i = 0; i < bonds.size(); i++) {
      const Bond &bond = bonds[i];
      int a = bond.atom_id1;
      int b = bond.atom_id2;
      if(atoms[a].getType().getName().find("H") == 0 or
	 atoms[b].getType().getName().find("H") == 0) uf.unite(a, b);
    }

    // get fragments
    vector<vector<int> > id_sets = uf.getSets();
    id_sets = uf.getSets();
    // atom_id to set_id
    vector<int> set_id(atoms.size());
    for(int i = 0; i < id_sets.size(); i++)
      for(int j = 0; j < id_sets[i].size(); j++)
	set_id[id_sets[i][j]] = i;

    // signle bond count (to find rotatable edge)
    vector<vector<Vector3d> > axises(atoms.size());
    for(int i = 0; i < bonds.size(); i++) {
      const Bond &bond = bonds[i];
      int a = bond.atom_id1;
      int b = bond.atom_id2;
      if(bond.is_rotor and
	 atoms[a].getType().getName() != "H" and
	 atoms[b].getType().getName() != "H" and
	 set_id[a] == set_id[b]) {
	Vector3d vec_a = atoms[b] - atoms[a];
	Vector3d vec_b = atoms[a] - atoms[b];
	vec_a /= vec_a.abs();
	vec_b /= vec_b.abs();
	axises[a].push_back(vec_a);
	axises[b].push_back(vec_b);
      }
    }
    // create edges
    vector<vector<FragmentEdge> > edgess(id_sets.size());
    vector<vector<int> > near_ids(id_sets.size());
    for(int i = 0; i < bonds.size(); i++) {
      const Bond &bond = bonds[i];
      int a = bond.atom_id1;
      int b = bond.atom_id2;
      if(set_id[a] != set_id[b]) {
	Vector3d vec_a = atoms[a];
	Vector3d vec_b = atoms[b];
	FragmentEdge edge_a(set_id[a], set_id[b], edgess[set_id[b]].size(), vec_a, vec_b);
	FragmentEdge edge_b(set_id[b], set_id[a], edgess[set_id[a]].size(), vec_b, vec_a);
	if(axises[a].size() == 1) edge_a.setAxis(axises[a][0]);
	if(axises[b].size() == 1) edge_b.setAxis(axises[b][0]);
	edgess[set_id[a]].push_back(edge_a);
	edgess[set_id[b]].push_back(edge_b);
	near_ids[set_id[a]].push_back(b);
	near_ids[set_id[b]].push_back(a);
      }
    }
    // create fragments
    vector<Fragment> fragments;
    for(int i = 0; i < id_sets.size(); i++) {
      vector<Atom> frag_atoms;
      for(int j = 0; j < id_sets[i].size(); j++)
	frag_atoms.push_back(atoms[id_sets[i][j]]);
      Fragment frag(i, edgess[i], frag_atoms, near_ids[i]);
      fragments.push_back(frag);
    }
    
    return fragments;
  }
}
