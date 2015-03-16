#include <iostream>

#include <opencv2/highgui.hpp>

#include "scene.h"
#include "sphereobject.h"

using namespace std;
using namespace d3Projection;
using namespace cv;

int main() {
    SphereObject s1(Vec<double, 3>(0, 0, 0), 1.0, Scalar(255, 0, 0));
    SphereObject s2(Vec<double, 3>(2, 0, 0), 1.0, Scalar(0, 255, 0));
    SphereObject s3(Vec<double, 3>(0, 2, 0), 1.0, Scalar(0, 0, 255));
    SphereObject s4(Vec<double, 3>(0, 10, 0), 1.0, Scalar(0, 255, 255));
    Camera cam(10, 120, 120, 500, 500);

    Scene scene;
    scene.addObject(&s1);
    scene.addObject(&s2);
    scene.addObject(&s3);
    scene.addObject(&s4);

    cam.watch(Vec3d(0, 0, 0));
    cam.setOrbit([](double theta) -> Vec3d {
        return Vec3d(-5*cos(theta), -5*sin(theta), 0.0);
    }, make_pair(0.0, 2*CV_PI), CV_PI, 0.03);

    while(true) {
        Mat res(cam.pixelsX, cam.pixelsY, CV_8UC3, cv::Scalar(0));
        scene.render(cam, res);
        imshow("output", res);
        waitKey(10);

        if(!cam.updatePosition()) {
            cam.updateOrbitParam(0.0);
        }
    }
    return 0;
}

