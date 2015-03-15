#ifndef SPHEREOBJECT_H
#define SPHEREOBJECT_H

#include <algorithm>
#include "isceneobject.h"

namespace d3Projection {

class SphereObject : public ISceneObject {
    cv::Scalar _color;
    double findRadiusProjected() const;
public:
    SphereObject(const cv::Vec3d &center, double radius, const cv::Scalar &color);
    void render(cv::Mat &outputImage) const;
};

}

#endif // SPHEREOBJECT_H
