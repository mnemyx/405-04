#include "Ray.h"
using namespace std;

RAY::RAY() {
	Vector v(0.0,0.0,0.0);
	Vector3d v2(v);
	
	rayO = v2;
	rayD = v2;
}

RAY::RAY(Vector3d v1, Vector3d v2) {
	rayO = v1;
	rayD = v2;
}


void RAY::set(Vector3d v1, Vector3d v2) {
	rayO = v1;
	rayD = v2;
}
