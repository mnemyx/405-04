#include "Vector.h"

#ifndef CAMERA_H
#define CAMERA_H


using namespace std;


class Camera
{
public:
    Vector3d vPoint;
    Vector3d vDir;
    Vector3d vUp;
    double focal;
    double aspect;
    Vector2d screen;

	Camera();
    Camera(Vector, Vector, Vector, double, double, double);
    void set(Vector, Vector, Vector, double, double, double);
    void set(Vector *, Vector *, Vector *, double, double, double);
    
};

#endif
