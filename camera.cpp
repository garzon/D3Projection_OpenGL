#include "camera.h"

using namespace d3Projection;

Camera::Camera(double focalLength, double _FOVX, double _FOVY, int _pixelsX, int _pixelsY) :
    hasOrbit(false),
    checkAng(false), checkPos(false), focalLen(focalLength),
    pixelsX(_pixelsX), pixelsY(_pixelsY), FOVX(_FOVX), FOVY(_FOVY)
{}

void Camera::setPosition(const cv::Vec3d &_pos) {
    pos = _pos;
    checkPos = true;
}

inline cv::Vec3d Camera::mat2Vec3d(const cv::Mat &expr) {
    const double *data = expr.ptr<double>();
    cv::Vec3d ret;
    ret[0] = data[0];
    ret[1] = data[1];
    ret[2] = data[2];
    return ret;
}

void Camera::setAngle(double _theta, double _phi) {
    theta = _theta; phi = _phi;
    double cosTheta = cos(theta), sinTheta = sin(theta);
    double cosPhi = cos(phi), sinPhi = sin(phi);

    focalVec[0] = cosTheta * cosPhi;
    focalVec[1] = sinTheta * cosPhi;
    focalVec[2] = sinPhi;
    focalVec = -focalVec * focalLen;

    baseX[0] = -sinTheta;
    baseX[1] = cosTheta;
    baseX[2] = 0;
    baseX = baseX * focalLen * tan(FOVX);

    baseY[0] = -cosTheta * sinPhi;
    baseY[1] = -sinTheta  * sinPhi;
    baseY[2] = cosPhi;
    baseY = -baseY * focalLen * tan(FOVY);

    checkAng = true;

}

bool Camera::check() const {
    return checkAng && checkPos;
}

void Camera::setOrbit(const std::function<cv::Vec3d (Camera *, double)> &equation, const std::pair<double, double> &paramRange, double init, double step) {
    _orbitFunc = equation;
    _orbitParamRange = paramRange;
    _step = step;
    _orbitParam = init;
    setPosition(_orbitFunc(this, init));
    hasOrbit = true;
}

void Camera::updateOrbitParam(double newParamVal) {
    _orbitParam = newParamVal;
}

bool Camera::updatePosition() {
    if(!hasOrbit) throw "Camera::updatePosition() - Exception: The camera does not have orbit.";
    _orbitParam += _step;
    if(_orbitParam < _orbitParamRange.first || _orbitParam > _orbitParamRange.second) {
        return false;
    }
    setPosition(_orbitFunc(this, _orbitParam));
    return true;
}
