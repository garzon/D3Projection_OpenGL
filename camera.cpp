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

    focalVec[0] = -cosTheta * cosPhi;
    focalVec[1] = -sinTheta * cosPhi;
    focalVec[2] = -sinPhi;
    focalVec = focalVec * focalLen;

    baseX[0] = -sinTheta;
    baseX[1] = cosTheta;
    baseX[2] = 0;
    baseX = baseX * focalLen * tan(FOVX);

    baseY[0] = -cosTheta * sinPhi;
    baseY[1] = -sinTheta  * sinPhi;
    baseY[2] = cosPhi;
    baseY = baseY * focalLen * tan(FOVY);

    checkAng = true;
}

bool Camera::check() const {
    return checkAng && checkPos;
}

void Camera::updateCameraParams() {
    setPosition(pos);
    setAngle(theta, phi);
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

bool Camera::updateOrbitPosition() {
    if(!hasOrbit) throw "Camera::updatePosition() - Exception: The camera does not have orbit.";
    _orbitParam += _step;
    if(_orbitParam < _orbitParamRange.first || _orbitParam > _orbitParamRange.second) {
        return false;
    }
    setPosition(_orbitFunc(this, _orbitParam));
    return true;
}


inline cv::Vec3d Camera::projectPoint(const cv::Vec3d &point) {
    if(!check()) throw "Camera::projectPoint() - Exception: The camera is not ready. Please set the parameters of the camera.";
    cv::Vec3d diff = point - pos;
    cv::Mat_<double> res(0, 0);
    for(int i = 0; i < 3; i++) {
        res.push_back(baseX[i]);
        res.push_back(baseY[i]);
        res.push_back(diff[i]);
    }
    res = res.reshape(1, 3).inv(cv::DECOMP_SVD) * cv::Mat(-focalVec);
    double *data = res.ptr<double>();
    cv::Vec3d ret;
    ret[0] = (data[0]+1) * 0.5 * pixelsX;
    ret[1] = (data[1]+1) * 0.5 * pixelsY;
    ret[2] = focalLen / data[2];
    return ret;
}

inline cv::Vec3d Camera::unprojectPoint(const cv::Vec3d &point) {
    if(!check()) throw "Camera::unprojectPoint() - Exception: The camera is not ready. Please set the parameters of the camera.";
    return pos - (focalVec + point[0] * baseX + point[1] * baseY) / point[2];
}

cv::Mat Camera::capture(Scene &scene, bool renderImage) {
    _renderedImage = cv::Mat::zeros(pixelsY, pixelsX, CV_8UC3);
    std::vector<ISceneObject *> &objs = scene.getObjs();

    for(auto obj: objs) {
        const std::vector<cv::Vec3d> &points = obj->getLocatingPoints();
        obj->clean();
        obj->lastCamera = this;
        for(const auto &point: points) {
            obj->pushLocatingPointProjected(projectPoint(point));
        }
    }
    std::sort(objs.begin(), objs.end(), [](ISceneObject *p, ISceneObject *q) -> bool {
        return (p->getLocatingPointsProjected()[0][2]) > (q->getLocatingPointsProjected()[0][2]);
    });

    if(!renderImage) return cv::Mat();
    for(auto obj: objs) {
        obj->render(_renderedImage);
    }

    return _renderedImage;
}
