#include <istream>

#include <openbabel/mol.h>
#include <openbabel/obconversion.h>

#include "OBMolReader.hpp"
#include "utils.hpp"

namespace format{
  std::vector<OpenBabel::OBMol> ParseFile(const std::string& filepath){
    std::ifstream ifs(filepath.c_str());
    if(ifs.fail()){
      std::cerr << "failed to open: " << filepath << std::endl; 
      abort();
    }
    std::string format = utils::GetExtension(filepath);
    return ParseFile(ifs, format);
  }
  std::vector<OpenBabel::OBMol> ParseFile(std::istream& stream, const std::string& in_format){
    OpenBabel::OBConversion conv(&stream);
    conv.SetInFormat(in_format.c_str());
    OpenBabel::OBMol mol;
    std::vector<OpenBabel::OBMol> molecules;
    for(bool end_frag = !conv.Read(&mol); end_frag != true; end_frag = !conv.Read(&mol)){
      mol.AddPolarHydrogens();
      molecules.push_back(mol);
    }

    return molecules;
  }


}
