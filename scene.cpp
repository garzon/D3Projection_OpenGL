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

float Scene::getDepth(int x, int y) const {
    if(depthImage.empty()) throw "Scene::getDepth() - Exception: Camera did not generate a depth image.";
    return depthImage.at<float>(x, y);
}
