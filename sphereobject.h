#ifndef SPHEREOBJECT_H
#define SPHEREOBJECT_H

#include <algorithm>
#include "isceneobject.h"

namespace d3Projection {

class SphereObject : public ISceneObject {
    cv::Scalar _color;
    float findRadiusProjected() const;
public:
    SphereObject(const cv::Vec3f&, float, cv::Scalar&);
    void render(cv::Mat&) const;
};

}

#endif // SPHEREOBJECT_H
