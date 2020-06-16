#include <iostream>
#include <vector>
#include <iterator>
#include <string>
#include <sstream> 
#include <algorithm> //std::find in decomposite

#include <openbabel/mol.h>
#include <openbabel/obconversion.h>
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>

#include "Fragment.hpp"
#include "MoleculeToFragments.hpp"
#include "Converter.hpp"
#include "log_writer_stream.hpp"

#include "OBMolReader.hpp"
#include "infile_reader.hpp"
#include "utils.hpp"

namespace {
  format::DockingConfiguration parseArgs(int argc, char **argv){

    // Definition of options
    using namespace boost::program_options;
    options_description options("Options");
    options_description hidden("Hidden options");
    positional_options_description pos_desc;
    options.add_options()
      ("help,h", "show help")
      ("conf-file,c", value<std::string>(), "configuration file")
      ("fragment,f", value<std::string>(), "fragment file (.mol2 file)")
      ("ligand,l", value<std::string>(), "ligand file")
      ("output,o", value<std::string>(), "output (annotated) ligand file (.sdf file)")
      ("log", value<std::string>(), "log file");
    options_description desc;
    desc.add(options).add(hidden);
    variables_map vmap;
    store(command_line_parser(argc, argv).
	  options(desc).positional(pos_desc).run(), vmap);
    notify(vmap);

    // showing help dialogue.
    if (vmap.count("help")){
      std::cout << "Usage: espresso_decompose [options]\n"
		<< options << std::endl;
      std::exit(0);
    }

    // there are insufficient inputs
    if (!vmap.count("conf-file")){
      if (!vmap.count("fragment") || !vmap.count("ligand") || !vmap.count("output")){
	std::cout << "too few arguments" << std::endl;
	std::cout << "Usage: espresso_decompose [options]\n"
		  << options << std::endl;
	std::exit(1);

      }
    }
    
    // parse input options and configuration file
    format::DockingConfiguration conf;
    if (vmap.count("conf-file"))  conf                 = format::ParseInFile(vmap["conf-file"].as<std::string>().c_str());
    if (vmap.count("ligand"))     conf.ligand_file     = vmap["ligand"].as<std::string>();
    if (vmap.count("fragment"))   conf.fragment_file   = vmap["fragment"].as<std::string>();
    if (vmap.count("output"))     conf.output_file     = vmap["output"].as<std::string>();
    if (vmap.count("log"))        conf.log_file        = vmap["log"].as<std::string>();

    return conf;
  }

  std::string obmol2Smiles(const OpenBabel::OBMol& c_mol){
    static OpenBabel::OBConversion conv;
    OpenBabel::OBMol& mol = const_cast<OpenBabel::OBMol&>(c_mol);
    conv.SetOutFormat("can"); //canonical smiles
    conv.AddOption("n", OpenBabel::OBConversion::OUTOPTIONS);

    return conv.WriteString(&mol, true);

  }

  void correctvalence(OpenBabel::OBMol* obmol){
    OpenBabel::OBConversion conv_mol2can, conv_can2mol;
    conv_mol2can.SetInFormat("mol");
    conv_can2mol.SetInFormat("can");
    conv_mol2can.SetOutFormat("can");
    conv_can2mol.SetOutFormat("mol");
    std::string mol_string = conv_can2mol.WriteString(obmol);
    conv_mol2can.ReadString(obmol, mol_string);
  }

  int decomposite(const std::vector<OpenBabel::OBMol>&  molecules,
		        std::vector<std::string>&       frag_smi_list,
		        std::vector<OpenBabel::OBMol>&  annotated_mols,
		        std::vector<OpenBabel::OBMol>&  fragments){

    format::Converter conv;

    // TODO Segmentation fault was occurred when this function was parallelized.
    for(int i=0; i<molecules.size(); ++i){
      std::vector<std::string> frag_smiles;
      fragdock::Molecule mol = conv.toFragmentMol(molecules[i]);
      std::vector<fragdock::Fragment> frags = fragdock::getFragments(mol);
      
      for(std::vector<fragdock::Fragment>::iterator fit=frags.begin(); fit!=frags.end(); ++fit){
	OpenBabel::OBMol obmol = conv.toOBMol(*fit, molecules[i], true);
	correctvalence(&obmol);
	std::string smiles = obmol2Smiles(obmol);
	frag_smiles.push_back(smiles);
	obmol.SetTitle(smiles);
	
	if(std::find(frag_smi_list.begin(), frag_smi_list.end(), smiles) == frag_smi_list.end()){
	  frag_smi_list.push_back(smiles);
	  fragments.push_back(obmol);
	}

	int frag_num = (std::find(frag_smi_list.begin(), frag_smi_list.end(), smiles) - frag_smi_list.begin());
      }

      // add fragment info to molecules
      OpenBabel::OBPairData* pair = new OpenBabel::OBPairData(); 
      pair->SetAttribute("fragment_info");
      pair->SetValue(boost::algorithm::join(frag_smiles, ","));
      OpenBabel::OBMol obmol = molecules[i];
      obmol.SetData(pair);
      annotated_mols.push_back(obmol);

      // show progress
      if((i+1)%1000==0){
	logs::lout << logs::info << (i+1) << " compounds were finished to decompose. The number of fragments is " << fragments.size();
      }
    }

    return fragments.size();
  }

  void outputmolecules(const std::vector<OpenBabel::OBMol>& molecules,
		       const std::string& filename){

    std::ofstream fout(filename.c_str());
    if(fout.fail()){
      std::cerr << "ERROR:outputMol2: opening file failed:" << filename << std::endl;
      abort();
    }

    OpenBabel::OBConversion obconv;
    std::string ext = utils::GetExtension(filename);
    obconv.SetOutFormat(utils::GetExtension(filename).c_str());
    for(std::vector<OpenBabel::OBMol>::const_iterator mit=molecules.begin();
	mit != molecules.end();
	mit++){
      obconv.Write(&(const_cast<OpenBabel::OBMol&>(*mit)), &fout);
    }

    fout.close();
  }

}

int main(int argc, char** argv){

  format::Converter conv;

  // config load
  format::DockingConfiguration config = parseArgs(argc, argv);

  // logging start
  if(config.log_file == ""){
    logs::lout.open((config.output_file + "__" + utils::getDate() + ".log").c_str());
  }else{
    logs::lout.open(config.log_file.c_str());
  }
  if(!logs::lout) {
    std::cerr << "error: log file cannot open." << std::endl;
    return 1;
  }

  // preparing ligand data
  std::vector<std::vector<int> > mol2frag_list;
  std::vector<OpenBabel::OBMol> fragments;
  std::vector<std::string> frag_smi_list;
  std::vector<OpenBabel::OBMol> annotated_mols;
  logs::lout << logs::info << "parse ligand file: " << config.ligand_file;
  std::vector<OpenBabel::OBMol> molecules = format::ParseFile(config.ligand_file);
  logs::lout << logs::info << "decomposite ligands into fragments";
  decomposite(molecules, frag_smi_list, annotated_mols, fragments);

  logs::lout << logs::info << "output ligands added fragment information at " << config.output_file;
  outputmolecules(annotated_mols, config.output_file);

  logs::lout << logs::info << "output fragments at " << config.fragment_file;
  outputmolecules(fragments,      config.fragment_file);


  return 0;
}
