#include "sphereobject.h"

using namespace d3Projection;

SphereObject::SphereObject(const cv::Vec3d &center, double _radius, const cv::Scalar &color) :
    radius(_radius)
{
    lastCamera = NULL;
    _color = color;
    locatingPoints.push_back(center);
}

void SphereObject::render(cv::Mat &outputImage) {
    if(lastCamera == NULL) throw "SphereObject::render - Exception: Please call Camera::capture first.";
    if(locatingPointsProjected[0][2] > 0) {
        double radiusProjected = radius * lastCamera->focalLen / locatingPointsProjected[0][2];
        cv::Size2f siz(radiusProjected / cv::norm(lastCamera->baseY) * lastCamera->pixelsY / 2.0,
                       radiusProjected / cv::norm(lastCamera->baseX) * lastCamera->pixelsX / 2.0);
        cv::Point2d coor(locatingPointsProjected[0][0], locatingPointsProjected[0][1]);
        cv::ellipse(outputImage, coor, siz, 0, 0, 360, _color, CV_FILLED);
    }
}

double SphereObject::getDepth(double x, double y) const {
    if(lastCamera == NULL) throw "SphereObject::getDepth - Exception: Please call Camera::capture first.";
    if(locatingPointsProjected[0][2] < 0) return -1;
    double coeff = 2.0 * locatingPointsProjected[0][2] / lastCamera->focalLen;
    double diffX = (x - locatingPointsProjected[0][0]) * coeff * cv::norm(lastCamera->baseX) / lastCamera->pixelsX,
           diffY = (y - locatingPointsProjected[0][1]) * coeff * cv::norm(lastCamera->baseY) / lastCamera->pixelsY;
    double diffSqr = diffX * diffX + diffY * diffY;
    double radiusSqr = radius * radius;
    if(diffSqr <= radiusSqr) {
        return locatingPointsProjected[0][2] - cv::sqrt(radiusSqr - diffSqr);
    } else return -1;
}
