#include "scene.h"

using namespace d3Projection;

int Scene::addObject(ISceneObject *obj) {
    objs.push_back(obj);
    return objs.size()-1;
}

void Scene::removeObject(int id) {
    objs.erase(objs.begin() + id);
}

inline cv::Vec2d Scene::transformPoint(const Camera &cam, const cv::Vec3d &point) {
    cv::Vec3d diff = point - cam.pos;
    cv::Mat res(0, 0, CV_64F);
    for(int i = 0; i < 3; i++) {
        res.push_back(cam.baseX[i]);
        res.push_back(diff[i]);
        res.push_back(cam.baseZ[i]);
    }
    res = res.reshape(1, 3).inv(cv::DECOMP_SVD) * cv::Mat(-cam.focalVec);
    double *data = res.ptr<double>();
    if(data[1] < 0) throw "Scene::transformPoint - Cannot see the point in the camera.";
    cv::Vec2d ret;
    ret[0] = (data[0]+1) * 0.5 * cam.pixelsX;
    ret[1] = (data[2]+1) * 0.5 * cam.pixelsY;
    return ret;
}

cv::Mat Scene::render(const Camera &cam) {
    cv::Mat res(cam.pixelsX, cam.pixelsY, CV_8UC3, cv::Scalar(0));
    for(auto obj: objs) {
        const std::vector<cv::Vec3d> &points = obj->getLocatingPoints();
        obj->clean();
        try {
            for(const auto &point: points) {
                obj->pushLocatingPointProjected(transformPoint(cam, point));
            }
            obj->render(res);
        } catch(...) {}
    }
    return res;
}
