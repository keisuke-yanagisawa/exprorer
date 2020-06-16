#include "AtomType.hpp"
#include "utils.hpp"

#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include <iostream>
namespace fragdock {
  std::map<std::string, int> AtomType::id_table;
  std::vector<std::string> AtomType::name_table;

  void AtomType::addId(const std::string &name) {
    if(id_table.find(name) == id_table.end()) {
      int next_id = id_table.size();
      id_table[name] = next_id;
      name_table.push_back(name);
    }
  }

  std::string AtomType::getName() const {
    if(0 <= id and id < name_table.size()) return name_table[id];
    return "NONE";
  }
}
