#ifndef PLANEOBJECT_H
#define PLANEOBJECT_H

#include "isceneobject.h"

namespace d3Projection {

class PlaneObject : public ISceneObject {
    cv::Scalar color;
public:
    PlaneObject(const cv::Vec3d &origin, const cv::Vec3d &vec1, const cv::Vec3d &vec2, const cv::Scalar &_color);
    void render(const Camera &cam, cv::Mat &outputImage) const;
};

}

#endif // PLANEOBJECT_H
