#include "sphereobject.h"

using namespace d3Projection;

SphereObject::SphereObject(const cv::Vec3d &center, double radius, const cv::Scalar &color) {
    _color = color;
    locatingPoints.push_back(center);
    for(int i = 0; i < 3; i++) {
        cv::Vec3d tmp = center;
        tmp[i]+=radius;
        locatingPoints.push_back(tmp);
    }
    for(int i = 0; i < 3; i++) {
        cv::Vec3d tmp = center;
        tmp[i]-=radius;
        locatingPoints.push_back(tmp);
    }
}

double SphereObject::findRadiusProjected() const {
    if(locatingPointsProjected.size() != 7) throw "SphereObject::findRadiusProjected() exception: locatingPointsProjected.size error.";
    double res = -1;
    for(int i = 1; i < 7; i++) {
        res = std::max(res, (double)cv::norm(locatingPointsProjected[0] - locatingPointsProjected[i]));
    }
    return res;
}

void SphereObject::render(cv::Mat &outputImage) const {
    double radius = findRadiusProjected();
    cv::circle(outputImage, cv::Point2f(locatingPointsProjected[0]), (int)radius, _color, CV_FILLED);
}
