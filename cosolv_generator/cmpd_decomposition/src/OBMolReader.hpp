#ifndef OBMOL_READER_H_
#define OBMOL_READER_H_

#include <istream>

#include <openbabel/mol.h>

namespace format{
  std::vector<OpenBabel::OBMol> ParseFile(const std::string& filepath);
  std::vector<OpenBabel::OBMol> ParseFile(std::istream& stream, const std::string& in_format);
}

#endif
