#include "camera.h"

using namespace d3Projection;

Camera::Camera(double focalLength, double _FOVX, double _FOVY, int _pixelsX, int _pixelsY) :
    checkAng(false), checkPos(false), focalLen(focalLength),
    pixelsX(_pixelsX), pixelsY(_pixelsY), FOVX(_FOVX), FOVY(_FOVY)

{}

void Camera::setPosition(const cv::Vec3d &_pos) {
    pos = _pos;
    checkPos = true;
}

void Camera::_GramSchmidt() {
    // TODO: Gram-Schmidt method
    // buggy method now

    cv::Vec3d aim1;
    aim1[0] = -33.436346235;
    aim1[1] = 24.637823524;
    aim1[2] = 16.425238545;

    cv::Vec3d aim2;
    aim2[0] = 15.2456235;
    aim2[1] = -32.64563724;
    aim2[2] = -23.56246845;

    cv::Vec3d n1 = focalVec / cv::norm(focalVec);
    baseX = aim1 - aim1.dot(n1) * n1;
    cv::Vec3d n2 = baseX / cv::norm(baseX);
    baseZ = aim2 - aim2.dot(n1) * n1 - aim2.dot(n2) * n2;

    baseX = n2 * focalLen * tan(FOVX);
    baseZ = baseZ * focalLen * tan(FOVY) / cv::norm(baseZ);
}

void Camera::setAngle(double _theta, double _phi) {
    theta = _theta; phi = _phi;
    focalVec[0] = cos(theta)*cos(phi);
    focalVec[1] = sin(theta)*cos(phi);
    focalVec[2] = sin(phi);
    focalVec = - focalVec * focalLen;
    checkAng = true;
    _GramSchmidt();
}

bool Camera::check() const {
    return checkAng && checkPos;
}
