#include "scene.h"

using namespace d3Projection;

int Scene::addObject(ISceneObject *obj) {
    objs.push_back(obj);
    return objs.size()-1;
}

void Scene::removeObject(int id) {
    objs.erase(objs.begin() + id);
}

inline cv::Vec3d Scene::transformPoint(const Camera &cam, const cv::Vec3d &point) {
    cv::Vec3d diff = point - cam.pos;
    cv::Mat_<double> res(0, 0);
    for(int i = 0; i < 3; i++) {
        res.push_back(cam.baseX[i]);
        res.push_back(cam.baseY[i]);
        res.push_back(diff[i]);
    }
    res = res.reshape(1, 3).inv(cv::DECOMP_SVD) * cv::Mat(-cam.focalVec);
    double *data = res.ptr<double>();
    cv::Vec3d ret;
    ret[0] = (data[0]+1) * 0.5 * cam.pixelsX;
    ret[1] = (data[1]+1) * 0.5 * cam.pixelsY;
    ret[2] = data[2];
    return ret;
}

cv::Mat Scene::render(const Camera &cam) {
    cv::Mat res(cam.pixelsX, cam.pixelsY, CV_8UC3, cv::Scalar(0));
    for(auto obj: objs) {
        const std::vector<cv::Vec3d> &points = obj->getLocatingPoints();
        obj->clean();
        for(const auto &point: points) {
            obj->pushLocatingPointProjected(transformPoint(cam, point));
        }
    }
    std::sort(objs.begin(), objs.end(), [](ISceneObject *p, ISceneObject *q) -> bool {
        return (p->getLocatingPointsProjected()[0][2]) < (q->getLocatingPointsProjected()[0][2]);
    });
    for(auto obj: objs) {
        obj->render(cam, res);
    }
    return res;
}
