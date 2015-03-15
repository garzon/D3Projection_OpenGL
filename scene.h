#ifndef SCENE_H
#define SCENE_H

#include "isceneobject.h"
#include "camera.h"

namespace d3Projection {

class Scene {
    std::vector<ISceneObject> objs;
    class CameraInfo {
        inline void _calcInvMat(const Camera &cam);
        inline void _calcCamParam(const Camera &cam);
    public:
        cv::Mat invMat, camParam;
        int pixelsX, pixelsY;
        CameraInfo(const Camera &cam);
    };
    inline static cv::Vec2f _transformPoint(const CameraInfo &info, const cv::Vec3f &point);
public:
    Scene();
    int addObject(const ISceneObject&);
    void removeObject(int);
    cv::Mat render(const Camera&);
    inline static cv::Vec2f transformPoint(const Camera &cam, const cv::Vec3f &point);
    void transformObjects(const Camera &cam);
};

}

#endif // SCENE_H
