#ifndef ISCENEOBJECT_H
#define ISCENEOBJECT_H

#include <vector>
#include <opencv2/opencv.hpp>

namespace d3Projection {

class ISceneObject {
protected:
    std::vector<cv::Vec2f> locatingPointsProjected;
    std::vector<cv::Vec3f> locatingPoints;

public:
    // method need to be overrided
    virtual void render(cv::Mat&) {
        throw "ISceneObject::render() called. You should override the method in child class.";
    }

    // -------------------------------

    const std::vector<cv::Vec3f>& getLocatingPoints() const;
    void pushLocatingPointProjected(const cv::Vec2f&);
    void clean();
};

}

#endif // ISCENEOBJECT_H
