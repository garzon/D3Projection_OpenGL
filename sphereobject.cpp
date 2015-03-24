#include "sphereobject.h"

using namespace d3Projection;

SphereObject::SphereObject(const cv::Vec3d &center, double _radius, const cv::Scalar &color) :
    radius(_radius)
{
    isProjected = false;
    _color = color;
    locatingPoints.push_back(center);
}

void SphereObject::render(const Camera &cam, cv::Mat &outputImage) {
    if(locatingPointsProjected[0][2] > 0) {
        double radiusProjected = radius * cam.focalLen / locatingPointsProjected[0][2];
        cv::Size2f siz(radiusProjected / cv::norm(cam.baseY) * cam.pixelsY / 2.0,
                       radiusProjected / cv::norm(cam.baseX) * cam.pixelsX / 2.0);
        cv::Point2d coor(locatingPointsProjected[0][0], locatingPointsProjected[0][1]);
        cv::ellipse(outputImage, coor, siz, 0, 0, 360, _color, CV_FILLED);
    }
}

double SphereObject::getDepth(const Camera &cam, double x, double y) const {
    if(!isProjected) throw "SphereObject::getDepth - Exception: Please call Camera::capture first.";
    if(locatingPointsProjected[0][2] < 0) return -1;
    double coeff = 2.0 * locatingPointsProjected[0][2] / cam.focalLen;
    double diffX = (x - locatingPointsProjected[0][0]) * coeff * cv::norm(cam.baseX) / cam.pixelsX,
           diffY = (y - locatingPointsProjected[0][1]) * coeff * cv::norm(cam.baseY) / cam.pixelsY;
    double diffSqr = diffX * diffX + diffY * diffY;
    double radiusSqr = radius * radius;
    if(diffSqr <= radiusSqr) {
        return locatingPointsProjected[0][2] - cv::sqrt(radiusSqr - diffSqr);
    } else return -1;
}
