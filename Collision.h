#include "Vector.h"

using namespace std;

#ifndef COLLISION_H
#define COLLISION_H

using namespace std;

class Collision
{
public: 
    double t;
    Vector3d hitpoint;
    Vector3d normal;
    
    Collision();
    Collision(double, Vector, Vector);
    void set(double, Vector3d, Vector3d);
    void set(Collision);
    
};

#endif
