#include "sphereobject.h"

using namespace d3Projection;

SphereObject::SphereObject(const cv::Vec3d &center, double _radius, const cv::Scalar &color) :
    radius(_radius)
{
    lastCamera = NULL;
    _color = color;
    locatingPoints.push_back(center);
}

void SphereObject::render() {
    if(lastCamera == NULL) throw "SphereObject::render - Exception: Please call Camera::capture first.";

    glColor3f(_color[0]/255.0, _color[1]/255.0, _color[2]/255.0);
    glTranslatef(locatingPoints[0][0], locatingPoints[0][1], locatingPoints[0][2]);
    gluSphere(gluNewQuadric(), radius, 10, 10);
}

