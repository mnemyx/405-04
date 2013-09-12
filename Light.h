#include "Vector.h"
#include "Color.h"

#ifndef LIGHT_H
#define LIGHT_H


using namespace std;


class Light
{
public:
	int type;  //type 0 = parallel; type 1 = regular light; type 3 = spotlight
    Vector3d info;
    COLOR_C color;

	Light();
    Light(int, Vector*, COLOR_C*);
    void set(int, Vector*, COLOR_C*);
    void set(int, Vector3d, COLOR_C*);
};

#endif
