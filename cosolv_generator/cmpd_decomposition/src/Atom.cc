#include "Atom.hpp"

namespace fragdock {

  std::ostream& operator<< (std::ostream& os, const Atom& atom){
    os << "AtomId: " << atom.getId() << " "
       << "AtomType: " << atom.getType().getName() << " "
       << "Coordination: (" << atom.x << "," << atom.y << "," << atom.z << ") ";
      return os;
  }
}
