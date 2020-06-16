#include "common.hpp"
#include <vector>
#include <string>
#include <map>
#include <iostream>
#ifndef ATOM_TYPE_H_
#define ATOM_TYPE_H_

namespace fragdock {

  class AtomType {
    int id;
  public:
    static std::map<std::string, int> id_table;
    static std::vector<std::string> name_table;
    static void addId(const std::string &name);
    AtomType() : id(-1) {}
    AtomType(const std::string &name) { addId(name); id = id_table[name]; }
    int getId() const { return id; }
    std::string getName() const;
  };
}
#endif
