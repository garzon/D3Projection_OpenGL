#ifndef CAMERA_H
#define CAMERA_H

#include <cmath>
#include <opencv2/opencv.hpp>

namespace d3Projection {

class Camera {
    bool checkPos, checkAng;
public:
    float theta, phi; // angles of camera
    float focalLen;
    int pixelsX, pixelsY;
    cv::Vec3f pos, focalVec, baseX, baseZ;

    Camera(float focalLength, float FOVX, float FOVY);
    void setAngle(float, float);
    void setPosition(const cv::Vec3f&);
};

}

#endif // CAMERA_H
