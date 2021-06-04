#include "common.hpp"
#include <string>
#include <vector>

#ifndef INFILE_READER_H_
#define INFILE_READER_H_
namespace format {
  struct SearchGrid {
    fltype cx, cy, cz;
    fltype width_x, width_y, width_z;
    fltype pitch_x, pitch_y, pitch_z;
  };

  struct DockingConfiguration {
    SearchGrid grid;
    std::string receptor_file, beam_width, output_file, fragment_file, ligand_file; 
    std::string log_file, forcefield_file, grid_folder, rotangs_file;
    int capping_atomic_num;
    bool do_carbon_capping;
  };

  DockingConfiguration ParseInFile(const char *filename);
} // namespace format

#endif
