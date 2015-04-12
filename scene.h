#ifndef SCENE_H
#define SCENE_H

#include <algorithm>

#include "isceneobject.h"

namespace d3Projection {

class Scene {
    std::vector<ISceneObject *> objs;
    cv::Mat depthImage;
public:
    int addObject(ISceneObject *obj);
    void removeObject(int id);
    std::vector<ISceneObject *> & getObjs();
    float getDepth(int x, int y) const;
};

}

#endif // SCENE_H
