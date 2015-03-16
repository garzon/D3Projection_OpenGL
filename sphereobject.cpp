#include "sphereobject.h"

using namespace d3Projection;

SphereObject::SphereObject(const cv::Vec3d &center, double _radius, const cv::Scalar &color) :
    radius(_radius)
{
    _color = color;
    locatingPoints.push_back(center);
}

void SphereObject::render(const Camera &cam, cv::Mat &outputImage) const {
    if(locatingPointsProjected[0][2] > 0) {
        double radiusProjected = radius * locatingPointsProjected[0][2];
        cv::Size2f siz(radiusProjected / cv::norm(cam.baseY) * cam.pixelsY / 2.0,
                       radiusProjected / cv::norm(cam.baseX) * cam.pixelsX / 2.0);
        cv::Point2d coor(locatingPointsProjected[0][0], locatingPointsProjected[0][1]);
        cv::ellipse(outputImage, coor, siz, 0, 0, 360, _color, CV_FILLED);
    }
}
