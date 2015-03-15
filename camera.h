#ifndef CAMERA_H
#define CAMERA_H

#include <cmath>
#include <opencv2/opencv.hpp>

namespace d3Projection {

class Camera {
    bool checkPos, checkAng;
    void _GramSchmidt();
public:
    double theta, phi; // angles of camera
    double focalLen, FOVX, FOVY;
    int pixelsX, pixelsY;
    cv::Vec3d pos, focalVec, baseX, baseZ;

    Camera(double focalLength, double _FOVX, double _FOVY, int _pixelsX, int _pixelsY);
    void setAngle(double _theta, double _phi);
    void setPosition(const cv::Vec3d &_pos);
    bool check() const;
};

}

#endif // CAMERA_H
