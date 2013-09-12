#include "Color.h"
#include "Ray.h"
#include "Material.h"
#include "Collision.h"
#include "Vector.h"

#ifndef SPHERE_H
#define SPHERE_H


using namespace std;

class SPHERE_T
{
public:
    Vector3d ctr;
    double rad;
	Material material;
	
	SPHERE_T();
    SPHERE_T(Vector*, double, Material*);
	void set(Vector*, double, Material*);
	
    //int intsct(RAY_T, PT_T *, PT_T *, double *);
	//int spCheck(rayInfo, Vector*, Vector*, double*);
	int spCheck(RAY, Collision*);
};

#endif
