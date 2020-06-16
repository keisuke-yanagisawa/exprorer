#include "common.hpp"
#include "Vector3d.hpp"
#ifndef FRAGMENT_EDGE_H_
#define FRAGMENT_EDGE_H_

namespace fragdock {
  struct FragmentEdge {
    int from, to, rev_id;
    Vector3d fvec, tvec;
    bool rotatable;
    Vector3d axis;
    FragmentEdge() : from(-1), to(-1), rev_id(-1), rotatable(false) {}
    FragmentEdge(int from, int to, int rev_id, const Vector3d &fvec, const Vector3d &tvec) : from(from), to(to), rev_id(rev_id), fvec(fvec), tvec(tvec), axis(Vector3d()), rotatable(false) {}
    void setAxis(const Vector3d& axis) { this->axis = axis; rotatable = true; }
    void translate(const Vector3d &vec) { fvec.translate(vec); tvec.translate(vec); }
    void axisRotate(const Vector3d &axis, fltype theta) { fvec.axisRotate(axis, theta); tvec.axisRotate(axis, theta); }
    void rotate(const Rotation &rot) { fvec.rotate(rot); tvec.rotate(rot); }
  };
}
#endif
