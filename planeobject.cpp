#include "planeobject.h"

using namespace d3Projection;

PlaneObject::PlaneObject(const cv::Vec3d &origin, const cv::Vec3d &vec1, const cv::Vec3d &vec2, const cv::Scalar &_color) :
    color(_color)
{
    locatingPoints.push_back(origin);
    locatingPoints.push_back(origin+vec1);
    locatingPoints.push_back(origin+vec1+vec2);
    locatingPoints.push_back(origin+vec2);
}

void PlaneObject::render(const Camera &cam, cv::Mat &outputImage) const {

}
