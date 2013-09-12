#include "Collision.h"

using namespace std;

Collision::Collision() {
	Vector v(0.0,0.0,0.0);
	Vector3d v2(v);
	
	t = 999999; 
    hitpoint = v2;
    normal = v2;
}  
  

Collision::Collision(double thit, Vector hitloc, Vector hitnorm){
	Vector3d hitv(hitloc);
	Vector3d hitn(hitnorm);
	
	t = thit;
	hitpoint = hitv;
	normal = hitn;
}

void Collision::set(double thit, Vector3d hitloc, Vector3d hitnorm){
	t = thit;
	hitpoint = hitloc;
	normal = hitnorm;
}

void Collision::set(Collision col){
	t = col.t;
	hitpoint = col.hitpoint;
	normal = col.normal;
}
