#ifndef ISCENEOBJECT_H
#define ISCENEOBJECT_H

#include <vector>
#include <opencv2/opencv.hpp>

namespace d3Projection {

class Camera;

class ISceneObject {
protected:
    std::vector<cv::Vec3d> locatingPointsProjected;
    std::vector<cv::Vec3d> locatingPoints;
public:
    const Camera *lastCamera;

    // methods need to be overrided
    virtual void render(cv::Mat &outputImage) {
        throw "ISceneObject::render() called. You should override the method in child class.";
    }

    virtual double getDepth(double x, double y) const {
        throw "ISceneObject::getDepth() called. You should override the method in child class.";
    }

    // -------------------------------

    const std::vector<cv::Vec3d>& getLocatingPoints() const;
    const std::vector<cv::Vec3d>& getLocatingPointsProjected() const;
    void pushLocatingPointProjected(const cv::Vec3d &point);
    void clean();
};

}

#endif // ISCENEOBJECT_H
