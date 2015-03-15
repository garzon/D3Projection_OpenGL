#include "scene.h"

using namespace d3Projection;

Scene::CameraInfo::CameraInfo(const Camera &cam) :
    invMat(0, 0, CV_32F), pixelsX(cam.pixelsX), pixelsY(cam.pixelsY)
{
    _calcInvMat(cam);
    _calcCamParam(cam);
}

inline void Scene::CameraInfo::_calcInvMat(const Camera &cam) {
    for(int i = 0; i < 3; i++) {
        invMat.push_back(cam.baseX[i]);
        invMat.push_back(cam.baseZ[i]);
    }
    invMat.reshape(1, 3); // 3*2 Mat
    invMat = invMat.inv();
}

inline void Scene::CameraInfo::_calcCamParam(const Camera &cam) {
    camParam = invMat * cv::Mat(cam.pos - cam.focalVec);
}

int Scene::addObject(const ISceneObject &obj) {
    objs.push_back(obj);
    return objs.size()-1;
}

void Scene::removeObject(int id) {
    objs.erase(objs.begin() + id);
}

inline cv::Vec2f Scene::_transformPoint(const CameraInfo &info, const cv::Vec3f &point) {
    cv::Mat res = info.camParam - info.invMat * cv::Mat(point);
    float *data = res.ptr<float>();
    cv::Vec2f ret;
    ret[0] = data[0] * info.pixelsX;
    ret[1] = data[1] * info.pixelsY;
    return ret;
}

inline cv::Vec2f Scene::transformPoint(const Camera &cam, const cv::Vec3f &point) {
    return _transformPoint(CameraInfo(cam), point);
}

void Scene::transformObjects(const Camera &cam) {
    CameraInfo info(cam);
    for(auto &obj: objs) {
        const std::vector<cv::Vec3f> &points = obj.getLocatingPoints();
        obj.clean();
        for(const auto &point: points) {
            obj.pushLocatingPointProjected(_transformPoint(info, point));
        }
    }
}

cv::Mat Scene::render(const Camera &cam) {
    cv::Mat res;
    transformObjects(cam);
    for(const auto &obj: objs) {
        obj.render(res);
    }
    return res;
}
