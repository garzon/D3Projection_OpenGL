#ifndef CAMERA_H
#define CAMERA_H

#include <cmath>
#include <functional>
#include <opencv2/opencv.hpp>

namespace d3Projection {

class Camera {
    // stat
    bool checkPos, checkAng, isLocked, hasOrbit;

    // model method
    cv::Vec3d _aim1, _aim2;
    void _GramSchmidt();

    // orbit
    std::function<cv::Vec3d (double)> _orbitFunc;
    std::pair<double, double> _orbitParamRange;
    double _step, _orbitParam;
public:
    // params of the camera
    double theta, phi; // angles of camera
    double focalLen, FOVX, FOVY;
    int pixelsX, pixelsY;
    cv::Vec3d pos, focalVec, baseX, baseY, watchPoint;

    // constructor
    Camera(double focalLength, double _FOVX, double _FOVY, int _pixelsX, int _pixelsY);

    // basic setter
    void setAngle(double _theta, double _phi);
    void setPosition(const cv::Vec3d &_pos);

    // helper
    bool check() const;
    inline static cv::Vec3d mat2Vec3d(const cv::Mat &expr);

    // watch
    void watch(const cv::Vec3d &_watchPoint);
    void unwatch();

    // orbit
    void setOrbit(const std::function<cv::Vec3d (double)> &equation, const std::pair<double, double> &paramRange, double init, double step);
    void updateOrbitParam(double newParamVal);
    bool updatePosition();
};

}

#endif // CAMERA_H
