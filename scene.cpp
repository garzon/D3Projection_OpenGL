#include "scene.h"

using namespace d3Projection;

int Scene::addObject(ISceneObject *obj) {
    objs.push_back(obj);
    return objs.size()-1;
}

void Scene::removeObject(int id) {
    objs.erase(objs.begin() + id);
}

std::vector<ISceneObject *> & Scene::getObjs() {
    return objs;
}

double Scene::getDepth(double x, double y, ISceneObject **ret) const {
    // return negative number if no depth @ (x, y)
    if(ret != NULL) *ret = NULL;
    double res = -1, tmp;
    for(auto obj: objs) {
        tmp = obj->getDepth(x, y);
        if(res < 0 || (tmp >= 0 && tmp < res)) {
            res = tmp;
            if(ret != NULL) *ret = obj;
        }
    }
    if(ret != NULL && res < 0) *ret = NULL;
    return res;
}
