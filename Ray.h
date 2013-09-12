#include "Vector.h"

using namespace std;

#ifndef RAY_H
#define RAY_H

using namespace std;

class RAY
{
public: 
    Vector3d rayO, rayD;
    
    RAY();
    RAY(Vector3d, Vector3d);
    void set(Vector3d, Vector3d);
    
};

#endif
