#include "Sphere.h"

/** 
    Vector3d ctr;
    double rad;
	Material material;
	
	SPHERE_T();
    SPHERE_T(Vector, double, Material);
 **/

using namespace std;
SPHERE_T::SPHERE_T() {
	Vector temp(0.0,0.0,0.0);
	Vector3d cen(temp);
	Material mat();
	COLOR_C black(0.0,0.0,0.0);
	
	ctr = cen;
	rad = 1.0;
	material.set(&black, &black, &black, &black, 0.0, 1.0);
}

SPHERE_T::SPHERE_T(Vector *cen, double r, Material *mat){
	Vector3d temp(*cen); 
	
	ctr = temp;
	rad = r;
	material.set(*mat);
}

void SPHERE_T::set(Vector *cen, double r, Material *mat){
	Vector3d temp(*cen); 
	
	ctr = temp;
	rad = r;
	material.set(*mat);
}


// determines if the ray intersects the sphere or not
int SPHERE_T::spCheck(RAY ray, Collision *collide) {
    double a, b, c;
    double disc, t1, t2, t;
    Vector tempNorm(3);
    
    // for finding the sphere
    a = ray.rayD * ray.rayD;

	b = 2 * (ray.rayD * (ray.rayO - ctr));
	
    /**
    b = 2 * ( (ray.rayD.x * (ray.rayO.x - ctr.x)) +
              (ray.rayD.y * (ray.rayO.y - ctr.y)) +
              (ray.rayD.z * (ray.rayO.z - ctr.z)) );
    **/

    c = pow( (ray.rayO.x - ctr.x), 2) +
		pow( (ray.rayO.y - ctr.y), 2) +
		pow( (ray.rayO.z - ctr.z), 2) -
		rad*rad;
    /**
    c = pow( (arbRay.rayO[0] - ctr[0]), 2) +
		pow( (arbRay.rayO[1] - ctr[1]), 2) +
		pow( (arbRay.rayO[2] - ctr[2]), 2) -
		pow(rad, 2);
	**/
    
   // computing the discriminant
   disc = ((b*b) - (4 * a * c));

   if (disc <= 0) {
      return (-1);

   } else {
      t1 = (-b - sqrt(disc)) / (2 * a);
      t2 = (-b + sqrt(disc)) / (2 * a);
      
      if (t1 <= 0 && t2 <= 0) {
          return (-1);
      
      } else if ((t1 > 0 && t2 <= 0) || (t1 > 0 && t2 > 0 && t1 < t2)) {
          t = t1;

      } else {
          t = t2;
      } 

   
      // finding intersection point
      Vector temp(0.0,0.0,0.0);
      Vector3d interP(temp);
      
      interP = ray.rayO + (t * ray.rayD);
					 

      //finding surface normal
      Vector3d surfaceNorm(temp);
      
      surfaceNorm = (interP - ctr) / rad;
      surfaceNorm = surfaceNorm.normalize();
      
      collide->set(t, interP, surfaceNorm);
      
      return(1);
   }
}

