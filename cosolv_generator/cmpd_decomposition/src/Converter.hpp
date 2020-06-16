#include "common.hpp"
#include "Molecule.hpp"

#include <openbabel/mol.h>

#include <boost/format.hpp>
#ifndef CONVERTER_H_
#define CONVERTER_H_
namespace format {
  class Converter {
  public:
    Converter() {}
    fragdock::Molecule    toFragmentMol(const OpenBabel::OBMol& mol);
    OpenBabel::OBMol      toOBMol(const fragdock::Molecule &mol, 
				  const OpenBabel::OBMol& original_obmol, 
				  bool put_methyl_group);
  };
} // namespace format
#endif
