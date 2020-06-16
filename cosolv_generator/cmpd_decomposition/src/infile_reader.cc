#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include "infile_reader.hpp"

namespace format {
DockingConfiguration ParseInFile(const char *filename){
  std::ifstream ifs(filename);
  if (ifs.fail()){
    std::cerr << "opening grid file failed:" << filename << std::endl;
    abort();
  }
  DockingConfiguration conf;
  std::string buffer;
  while(!ifs.eof()){
    std::getline(ifs, buffer);
    if(boost::algorithm::starts_with(buffer, "BOX_SIZE ")){
      std::string str = buffer.substr(9);
      std::vector<std::string> vals;
      boost::algorithm::split(vals, str, boost::algorithm::is_any_of(","));
      boost::algorithm::trim(vals[0]);
      boost::algorithm::trim(vals[1]);
      boost::algorithm::trim(vals[2]);
      conf.grid.width_x = boost::lexical_cast<fltype>(vals[0]);
      conf.grid.width_y = boost::lexical_cast<fltype>(vals[1]);
      conf.grid.width_z = boost::lexical_cast<fltype>(vals[2]);
    }

    if(boost::algorithm::starts_with(buffer, "BOX_CENTER ")){
      std::string str = buffer.substr(11);
      std::vector<std::string> vals;
      boost::algorithm::split(vals, str, boost::algorithm::is_any_of(","));
      boost::algorithm::trim(vals[0]);
      boost::algorithm::trim(vals[1]);
      boost::algorithm::trim(vals[2]);
      conf.grid.cx = boost::lexical_cast<fltype>(vals[0]);
      conf.grid.cy = boost::lexical_cast<fltype>(vals[1]);
      conf.grid.cz = boost::lexical_cast<fltype>(vals[2]);
    }

    if(boost::algorithm::starts_with(buffer, "GRID_PITCH ")){
      std::string str = buffer.substr(11);
      std::vector<std::string> vals;
      boost::algorithm::split(vals, str, boost::algorithm::is_any_of(","));
      boost::algorithm::trim(vals[0]);
      boost::algorithm::trim(vals[1]);
      boost::algorithm::trim(vals[2]);
      conf.grid.pitch_x = boost::lexical_cast<fltype>(vals[0]);
      conf.grid.pitch_y = boost::lexical_cast<fltype>(vals[1]);
      conf.grid.pitch_z = boost::lexical_cast<fltype>(vals[2]);
    }

    if(boost::algorithm::starts_with(buffer, "RECEPTOR ")){
      conf.receptor_file = buffer.substr(9);
    }

    if(boost::algorithm::starts_with(buffer, "LIGAND ")){
      conf.ligand_file = buffer.substr(7);
    }
    if(boost::algorithm::starts_with(buffer, "OUTPUT ")){
      conf.output_file = buffer.substr(7);
    }
    if(boost::algorithm::starts_with(buffer, "FRAGMENT ")){
      conf.fragment_file = buffer.substr(9);
    }
    if(boost::algorithm::starts_with(buffer, "LOG ")){
      conf.log_file = buffer.substr(4);
    }
    if(boost::algorithm::starts_with(buffer, "FORCEFIELD ")){
      conf.forcefield_file = buffer.substr(11);
    }
    if(boost::algorithm::starts_with(buffer, "GRID_FOLDER ")){
      conf.grid_folder = buffer.substr(12);
    }
    if(boost::algorithm::starts_with(buffer, "ROTANGS ")){
      conf.rotangs_file = buffer.substr(8);
    }
  }
  return conf;
}
} // namespace format
