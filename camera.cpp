#include "camera.h"

using namespace d3Projection;

Camera::Camera(float focalLength, float FOVX, float FOVY, int _pixelsX, int _pixelsY) :
    checkAng(false), checkPos(false), focalLen(focalLength),
    pixelsX(_pixelsX), pixelsY(_pixelsY)
{
    // TODO: Gram－Schmidt method
}

void Camera::setPosition(const cv::Vec3f &_pos) {
    pos = _pos;
    checkPos = true;
}

void Camera::setAngle(float _theta, float _phi) {
    theta = _theta; phi = _phi;
    focalVec[0] = cos(theta)*cos(phi);
    focalVec[1] = sin(theta)*cos(phi);
    focalVec[2] = sin(phi);
    focalVec = focalVec * focalLen;
    checkAng = true;
}
