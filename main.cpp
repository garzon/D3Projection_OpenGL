#include <iostream>

#include <opencv2/highgui/highgui.hpp>

#include "camera.h"
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

    // orbit 1
    /*
    cam.setOrbit([](Camera *thisCam, double phi) -> Vec3d {
        thisCam->setAngle(CV_PI, -phi);
        return Vec3d(5*cos(phi), 0.0, 5*sin(phi));
    }, make_pair(0.0, 2*CV_PI), 0.0, 0.03);
    */

    // orbit 2

    cam.setOrbit([](Camera *thisCam, double theta) -> Vec3d {
        thisCam->setAngle(theta + CV_PI, 0);
        return Vec3d(5*cos(theta), 5*sin(theta), 0.0);
    }, make_pair(0.0, 2*CV_PI), 0.0, 0.01);


    while(true) {

        imshow("output", cam.capture(scene));
        cout << scene.getDepth(250, 250) << endl;
        waitKey(10);

        if(!cam.updateOrbitPosition()) {
            cam.updateOrbitParam(0.0);
        }
    }
    return 0;
}

