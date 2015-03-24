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
    double getDepth(const Camera &cam, double x, double y, ISceneObject **ret = NULL) const;
};

}

#endif // SCENE_H
