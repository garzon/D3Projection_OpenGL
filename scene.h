#ifndef SCENE_H
#define SCENE_H

#include <algorithm>

#include "isceneobject.h"
#include "camera.h"

namespace d3Projection {

class Scene {
    std::vector<ISceneObject*> objs;
public:
    int addObject(ISceneObject *obj);
    void removeObject(int id);
    cv::Mat render(const Camera &cam);
    inline static cv::Vec3d transformPoint(const Camera &cam, const cv::Vec3d &point);
};

}

#endif // SCENE_H
