#ifndef SPHEREOBJECT_H
#define SPHEREOBJECT_H

#include <algorithm>

#include "isceneobject.h"
#include "camera.h"

namespace d3Projection {

class SphereObject : public ISceneObject {
    cv::Scalar _color;
    double radius;
public:
    SphereObject(const cv::Vec3d &center, double _radius, const cv::Scalar &color);
    void render(const Camera &cam, cv::Mat &outputImage);
    double getDepth(const Camera &cam, double x, double y) const;
};

}

#endif // SPHEREOBJECT_H
