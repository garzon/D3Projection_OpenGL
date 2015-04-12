#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "camera.h"
#include "sphereobject.h"

using namespace std;
using namespace d3Projection;
using namespace cv;

int main(int argc, char **argv) {

    Scene scene;
    double x, y, z, theta, phi;

    glutInit(&argc, argv);

    SphereObject s1(Vec<double, 3>(0, 0, 0), 1.0, Scalar(255, 0, 0));
    SphereObject s2(Vec<double, 3>(2, 0, 0), 1.0, Scalar(0, 255, 0));
    SphereObject s3(Vec<double, 3>(0, 2, 0), 1.0, Scalar(0, 0, 255));
    SphereObject s4(Vec<double, 3>(0, 10, 0), 1.0, Scalar(255, 255, 255));

    scene.addObject(&s1);
    scene.addObject(&s2);
    scene.addObject(&s3);
    scene.addObject(&s4);


    FILE *f = fopen("/home/garzon/xyz.txt", "r");
    while(fscanf(f, "%lf %lf %lf", &x, &y, &z) > 0) {
        SphereObject *tmp = new SphereObject (Vec<double, 3>(x, y, z), 0.01, Scalar(255, 255, 255));
        scene.addObject(tmp);
    }
    fclose(f);


    Camera cam(90, 90, 700, 700);

    x=y=z=theta=phi=0;

    /*
    auto controller = [&](char c) {
        switch(c) {
            case 'a': x+=0.3; break;
            case 's': y+=0.3; break;
            case 'd': z+=0.3; break;
            case 'f': theta+=0.03; break;
            case 'g': phi+=0.03; break;
            case 'q': x-=0.3; break;
            case 'w': y-=0.3; break;
            case 'e': z-=0.3; break;
            case 'r': theta-=0.03; break;
            case 't': phi-=0.03; break;
        }
    };
    */

    // orbit 1

    cam.setOrbit([](Camera *thisCam, double phi) -> Vec3d {
        thisCam->setAngle(CV_PI, -phi);
        return Vec3d(10*cos(phi), 0.0, 10*sin(phi));
    }, make_pair(0.0, 2*CV_PI), 0.0, 0.03);


    // orbit 2
    /*
    cam.setOrbit([](Camera *thisCam, double theta) -> Vec3d {
        thisCam->setAngle(theta + CV_PI, 0);
        return Vec3d(5*cos(theta), 5*sin(theta), 0.0);
    }, make_pair(0.0, 2*CV_PI), 0.0, 0.01);
    */

    while(true) {

        cv::Mat tmp = cam.capture(scene, false);

        if(!tmp.empty()) imshow("output", tmp);

        char c = waitKey(1);

        //for(int i=0; i<2000*128*5; i++) float tmp = scene.getDepth(350,350);

        cout << scene.getDepth(350,350) <<endl;

        if(!cam.updateOrbitPosition()) {
            cam.updateOrbitParam(0.0);
        }
    }
    return 0;
}

