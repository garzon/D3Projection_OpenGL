#include "camera.h"

using namespace d3Projection;

Camera::Camera(double focalLength, double _FOVX, double _FOVY, int _pixelsX, int _pixelsY) :
    isLocked(false),
    checkAng(false), checkPos(false), focalLen(focalLength),
    pixelsX(_pixelsX), pixelsY(_pixelsY), FOVX(_FOVX), FOVY(_FOVY)
{

    _aim1[0] = -33.436346235;
    _aim1[1] = 24.637823524;
    _aim1[2] = 16.425238545;

    _aim2[0] = 15.2456235;
    _aim2[1] = -32.64563724;
    _aim2[2] = -23.56246845;
}

void Camera::setPosition(const cv::Vec3d &_pos) {
    pos = _pos;
    checkPos = true;
    if(isLocked) {
        cv::Vec3d diff = watchPoint - pos;
        double phi = atan(diff[2] / (0.0000001+sqrt(diff[1]*diff[1] + diff[0]*diff[0])));
        double theta = atan2(diff[1], diff[0]);
        if(theta < 0) theta = 2*CV_PI + theta;
        isLocked = false;
        setAngle(theta, phi);
        isLocked = true;
    }
}

inline cv::Vec3d Camera::mat2Vec3d(const cv::Mat &expr) {
    const double *data = expr.ptr<double>();
    cv::Vec3d ret;
    ret[0] = data[0];
    ret[1] = data[1];
    ret[2] = data[2];
    return ret;
}

void Camera::_GramSchmidt() {
    // TODO: Gram-Schmidt method
    // buggy method now

    cv::Mat_<double> rotateMat1(0, 0);
    rotateMat1.push_back(cos(theta));
    rotateMat1.push_back(-sin(theta));
    rotateMat1.push_back(0.0);

    rotateMat1.push_back(sin(theta));
    rotateMat1.push_back(cos(theta));
    rotateMat1.push_back(0.0);

    rotateMat1.push_back(0.0);
    rotateMat1.push_back(0.0);
    rotateMat1.push_back(1.0);
    rotateMat1 = rotateMat1.reshape(1, 3); // 3*3 Mat

    cv::Mat_<double> rotateMat2(0, 0);
    rotateMat2.push_back(cos(phi));
    rotateMat2.push_back(0.0);
    rotateMat2.push_back(sin(phi));

    rotateMat2.push_back(0.0);
    rotateMat2.push_back(1.0);
    rotateMat2.push_back(0.0);

    rotateMat2.push_back(sin(-phi));
    rotateMat2.push_back(0.0);
    rotateMat2.push_back(cos(phi));
    rotateMat2 = rotateMat2.reshape(1, 3); // 3*3 Mat

    cv::Vec3d aim1 = mat2Vec3d(rotateMat2 * rotateMat1 * cv::Mat(_aim1)),
              aim2 = mat2Vec3d(rotateMat2 * rotateMat1 * cv::Mat(_aim2));


    cv::Vec3d n1 = focalVec / cv::norm(focalVec);
    baseX = aim1 - aim1.dot(n1) * n1;
    cv::Vec3d n2 = baseX / cv::norm(baseX);
    baseY = aim2 - aim2.dot(n1) * n1 - aim2.dot(n2) * n2;

    baseX = n2 * focalLen * tan(FOVX);
    baseY = baseY * focalLen * tan(FOVY) / cv::norm(baseY);
}

void Camera::watch(const cv::Vec3d &_watchPoint) {
    watchPoint = _watchPoint;
    isLocked = true;
}

void Camera::unwatch() {
    isLocked = false;
}

void Camera::setAngle(double _theta, double _phi) {
    if(isLocked)
        throw "Camera::setAngle - exception: The camera is locked.";
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
