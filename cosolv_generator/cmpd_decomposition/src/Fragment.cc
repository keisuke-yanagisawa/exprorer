#include "Fragment.hpp"
#include "utils.hpp"

namespace {
  template<typename Container>
  bool exist_in(const Container& c, const typename Container::value_type& v){
    return ( c.end() != std::find(c.begin(),c.end(),v) );
  }
}

namespace fragdock {
  Fragment::Fragment(int id,
		     const std::vector<FragmentEdge>& edges,
		     const std::vector<Atom>& atoms,
		     const std::vector<int>& near_id) : id(id),
							edges(edges),
							near_id(near_id){
    this->atoms = atoms;
    //this->translate(-getCenter());
  }
  void Fragment::adjust(const FragmentEdge &redge) {
    std::cerr << "Fragment::adjust doesn't exist" << std::endl;
  }
  void Fragment::translate(const Vector3d& vec) {
    Molecule::translate(vec);
    for(int i = 0; i < edges.size(); i++)
      edges[i].translate(vec);
  }

  void Fragment::axisRotate(const Vector3d &axis, fltype theta, const std::vector<int>& id_set) {
    Molecule::axisRotate(axis, theta, id_set);
    for(int i = 0; i < edges.size(); i++){
      if(exist_in(id_set, edges[i].from) 
	 && exist_in(id_set, edges[i].to)){
	edges[i].axisRotate(axis, theta);
      }
    }
  }
  
  void Fragment::rotate(const Rotation& rot) {
    Molecule::rotate(rot);
    for(int i = 0; i < edges.size(); i++)
      edges[i].rotate(rot);
  }

}
