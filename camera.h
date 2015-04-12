#ifndef CAMERA_H
#define CAMERA_H

#include <cmath>
#include <functional>

#include <GL/glew.h>
#include <GL/glut.h>

#include <opencv2/opencv.hpp>

#include "scene.h"

namespace d3Projection {

class Camera {
    // stat
    bool checkPos, checkAng, hasOrbit;

    // orbit
    std::function<cv::Vec3d (Camera *, double)> _orbitFunc;
    std::pair<double, double> _orbitParamRange;
    double _step, _orbitParam;

    // cache
    cv::Mat _renderedImage;
public:
    // params of the camera
    double theta, phi; // angles of camera's len
    double FOVX, FOVY;
    int pixelsX, pixelsY;
    cv::Vec3d pos, focalVec, baseX, baseY;

    // constructor
    Camera(double _FOVX, double _FOVY, int _pixelsX, int _pixelsY);

    // basic setters
    void setAngle(double _theta, double _phi);
    void setPosition(const cv::Vec3d &_pos);

    // helpers
    bool check() const;
    void updateCameraParams();
    inline static cv::Vec3d mat2Vec3d(const cv::Mat &expr);
    inline cv::Vec3d projectPoint(const cv::Vec3d &point);
    inline cv::Vec3d unprojectPoint(const cv::Vec3d &point);

    // orbit
    void setOrbit(const std::function<cv::Vec3d (Camera *, double)> &equation, const std::pair<double, double> &paramRange, double init, double step);
    void updateOrbitParam(double newParamVal);
    bool updateOrbitPosition();

    // methods
    cv::Mat capture(Scene &scene, bool renderImage = true); // set to false to accelerate if rendered image is not needed
};

}

#endif // CAMERA_H
