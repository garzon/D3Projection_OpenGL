#include <iostream>

#include <opencv2/highgui.hpp>

#include "scene.h"
#include "sphereobject.h"

#define PI (3.14159265)

using namespace std;
using namespace d3Projection;
using namespace cv;

int main() {
    SphereObject s1(Vec<double, 3>(1, 0, 0), 1.0, Scalar(255, 255, 0));
    SphereObject s2(Vec<double, 3>(1, 1, 0), 1.0, Scalar(0, 255, 0));
    Camera cam(10, 120, 120, 500, 500);
    cam.setPosition(Vec<double, 3>(5, 0, 0));
    double theta = PI, phi = 0;

    Scene scene;
    scene.addObject(&s1); scene.addObject(&s2);
    while(theta > -PI) {
        theta -= 0.03;
        cam.setAngle(theta, phi);
        Mat res = scene.render(cam);
        imshow("output", res);
        waitKey(10);
    }
    return 0;
}

