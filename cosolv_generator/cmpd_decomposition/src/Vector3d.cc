#include "Vector3d.hpp"
#include "utils.hpp"

namespace {
  fltype getAxisAngle(fragdock::Vector3d poleA, fragdock::Vector3d poleB) {
    const fragdock::Vector3d e = fragdock::Vector3d(1.0, 0.0, 0.0);
    fragdock::Vector3d poleAxis = poleA.cross(e);
    poleAxis /= poleAxis.abs();
    fltype th = poleA.getAngle(e);
    poleA.axisRotate(poleAxis, th);
    poleB.axisRotate(poleAxis, th);
    return fragdock::Vector3d(0.0, 1.0, 0.0).getAngle(poleB);
  }
}


namespace fragdock {
  void Vector3d::axisRotate(const Vector3d &axis, double th) {
    Vector3d n = axis/axis.abs();
    fltype B = cos(th), A = 1-B, C = sin(th);
    fltype ox = x, oy = y, oz = z;
    x = (n.x*n.x*A +     B)*ox + (n.x*n.y*A - n.z*C)*oy + (n.z*n.x*A + n.y*C)*oz;
    y = (n.x*n.y*A + n.z*C)*ox + (n.y*n.y*A +     B)*oy + (n.y*n.z*A - n.x*C)*oz;
    z = (n.z*n.x*A - n.y*C)*ox + (n.y*n.z*A + n.x*C)*oy + (n.z*n.z*A +     B)*oz;
  }

  void Vector3d::rotate(const Rotation& rot) {
    using namespace std;
    if(std::abs(1.0 - rot.dir.x) < 1e-6) return;
    Vector3d e(1, 0, 0);
    this->axisRotate(e, rot.th);
    Vector3d crs = e.cross(rot.dir);
    //this->axisRotate(crs.unit(), acos(e.dot(rot.dir)/(e.abs()*rot.dir.abs())));
    this->axisRotate(crs.unit(), acos(rot.dir.x));
  }
  
  
  void Vector3d::rotate(fltype theta, fltype phi, fltype psi) {
    const fltype cosT = cos(theta), sinT = sin(theta), cosH = cos(phi), sinH = sin(phi), cosS = cos(psi), sinS = sin(psi);  
    fltype ox = x, oy = y, oz = z;

    x = (cosT*cosS  - sinT*cosH*sinS) * ox + (-cosT*sinS - sinT*cosH*cosS) * oy + (sinT*sinH ) * oz;
    y = (sinT*cosS  + cosT*cosH*sinS) * ox + (-sinT*sinS + cosT*cosH*cosS) * oy + (-cosT*sinH) * oz;
    z = (sinH*sinS                  ) * ox + (sinH*cosS                  ) * oy + (cosH      ) * oz;
  }

  std::vector<fragdock::Rotation> Rotation::makeRotations() {
    const fltype phi = (1.0 + sqrt(5.0)) / 2.0;
    const fltype pi = acos(-1.0);
    
    fragdock::Vector3d poleA(0, phi*phi*phi, phi*phi);
    fragdock::Vector3d poleB(0, 1.0, phi*phi);
    poleA = poleA.unit();
    poleB = poleB.unit();
    
    const fltype base_theta = getAxisAngle(poleA, poleB);
    
    const int ORDER_NUM = 12;
    const int order[ORDER_NUM] = {0, 0, 1, 1, 1, 2, 1, 2, 2, 2, 3, -1};
    
    std::vector<fragdock::Rotation> rots;
    for(int ord_id = 0; ord_id < ORDER_NUM; ord_id++) {
      int o = order[ord_id];
      for(int i = 0; i < 5; i++) {
	poleB.axisRotate(poleA, 2.0*pi/5.0);
	fltype theta = getAxisAngle(poleA, poleB);
	rots.push_back(fragdock::Rotation(poleA, theta - base_theta));
      }
      poleB.axisRotate(poleA, 2.0*o*pi/5.0);
      poleA.axisRotate(poleB, 4.0*pi/3.0);
    }    
    return rots;    
  }

  std::vector<fragdock::Rotation> Rotation::makeRotationsRandom() {    
    using namespace utils;
    std::vector<Rotation> rots;
    //logs::lout << logs::info << "================ random rotate ================" << std::endl;
    const fltype pi = acos(-1.0);
    const int ROT_NUM = 60;
    rots.push_back(Rotation(1.0, 0.0, 0.0, 0.0));
    for(int i = 0; i < ROT_NUM; i++) {
      fltype x = randf(-1.0, 1.0);
      fltype y = randf(-1.0, 1.0);
      fltype z = randf(-1.0, 1.0);
      fltype th = randf(0.0, 2.0*pi);
      Vector3d vec(x, y, z);
      rots.push_back(Rotation(vec.unit(), th));
    }
    return rots;
  }
  

}
