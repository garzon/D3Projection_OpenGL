#include "camera.h"

using namespace d3Projection;

Camera::Camera(double _FOVX, double _FOVY, int _pixelsX, int _pixelsY) :
    hasOrbit(false),
    checkAng(false), checkPos(false),
    pixelsX(_pixelsX), pixelsY(_pixelsY), FOVX(_FOVX), FOVY(_FOVY)
{
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);

    glutInitWindowSize(pixelsX, pixelsY);    //显示框大小

    glutInitWindowPosition(200,400); //确定显示框左上角的位置
    glutCreateWindow("Camera");

    glEnable(GL_DEPTH_TEST);

    glViewport(0, 0, pixelsX, pixelsY);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(FOVY, tan(FOVX/360.0*CV_PI)/tan(FOVY/360.0*CV_PI), zNear, zFar);

    FOVX = FOVX / 360.0 * CV_PI;
    FOVY = FOVY / 360.0 * CV_PI;

}

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

    baseY[0] = -cosTheta * sinPhi;
    baseY[1] = -sinTheta  * sinPhi;
    baseY[2] = cosPhi;
    baseY = baseY * tan(FOVY);

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

cv::Mat Camera::capture(Scene &scene, bool renderImage) {

    if(!check()) throw "Camera::capture() - Exception: Please set up params of the camera first.";

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);      // 黑色背景
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(pos[0], pos[1], pos[2], -focalVec[0], -focalVec[1], -focalVec[2], baseY[0], baseY[1], baseY[2]);

    std::vector<ISceneObject *> &objs = scene.getObjs();

    for(auto obj: objs) {
        obj->lastCamera = this;
        glPushMatrix();
        obj->render();
        glPopMatrix();
    }

    scene.depthImage = cv::Mat::zeros(pixelsY, pixelsX, CV_32F);
    glReadPixels(0, 0, pixelsX, pixelsY, GL_DEPTH_COMPONENT, GL_FLOAT, scene.depthImage.data);
    cv::flip(scene.depthImage, scene.depthImage, 0);
    scene.depthImage = - zFar * zNear / (scene.depthImage*(zFar-zNear) - zFar);

    if(!renderImage) {
        glFlush();
        return cv::Mat();
    }

    _renderedImage = cv::Mat(pixelsY, pixelsX, CV_8UC3);
    glReadPixels(0, 0, pixelsX, pixelsY, GL_BGR, GL_UNSIGNED_BYTE, _renderedImage.data);
    cv::flip(_renderedImage, _renderedImage, 0);

    glFlush();

    return _renderedImage;
}
