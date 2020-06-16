#include "common.hpp"
#include "Molecule.hpp"
#include "FragmentEdge.hpp"
#include "Atom.hpp"
#include <iostream>
#ifndef FRAGMENT_H_
#define FRAGMENT_H_
namespace fragdock {
  class Fragment : public Molecule {
    int id;
    std::vector<FragmentEdge> edges;
    std::vector<int> near_id;
  public:
    Fragment(int id, const std::vector<FragmentEdge> &edges, const std::vector<Atom> &atoms, const std::vector<int>& near_id);
    Fragment() {};
    int getId() const { return id; }
    void adjust(const FragmentEdge &redge);
    const FragmentEdge& getEdge(int i) const { return edges[i]; }
    const std::vector<FragmentEdge>& getEdges() const { return edges; }
    int getEdgeSize() { return edges.size(); }
    void translate(const Vector3d& vec);
    void axisRotate(const Vector3d &axis, fltype theta, const std::vector<int>& id_set);
    void rotate(const Rotation& rot);
  };
}
#endif
