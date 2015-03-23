#ifndef SCENE_H
#define SCENE_H

#include <algorithm>

#include "isceneobject.h"

namespace d3Projection {

class Scene {
    std::vector<ISceneObject *> objs;
public:
    int addObject(ISceneObject *obj);
    void removeObject(int id);
    std::vector<ISceneObject *> & getObjs();
};

}

#endif // SCENE_H
