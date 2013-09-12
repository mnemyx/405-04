#include "camera.h"

using namespace std;

Camera::Camera() {
	Vector temp(0.0,0.0,0.0);
	Vector temp2(0.0,0.0);
	Vector3d vp(temp);
	Vector2d sa(temp2);
	
	vPoint = vp;
	vDir = vp;
	vUp = vp;
	focal = 0.0;
	aspect = 0.0;
	screen = sa;
}

Camera::Camera(Vector v1, Vector v2, Vector v3, double f, double a, double sw) {
	Vector3d vp(v1);
	Vector3d vd(v2);
	Vector3d vu(v3);
	
	vPoint = vp;
	vDir = vd;
	vUp = vu;
	focal = f;
	aspect = a;
	
	Vector temp(sw, (sw/a));
	Vector2d ss(temp);
	screen = ss;
}

void Camera::set(Vector v1, Vector v2, Vector v3, double f, double a, double sw) {
	Vector3d vp(v1);
	Vector3d vd(v2);
	Vector3d vu(v3);
	
	vPoint = vp;
	vDir = vd;
	vUp = vu;
	focal = f;
	aspect = a;
	
	Vector temp(sw, (sw/a));
	Vector2d ss(temp);
	screen = ss;
}


void Camera::set(Vector *v1, Vector *v2, Vector *v3, double f, double a, double sw) {
	Vector3d vp(*v1);
	Vector3d vd(*v2);
	Vector3d vu(*v3);
	
	vPoint = vp;
	vDir = vd;
	vUp = vu;
	focal = f;
	aspect = a;
	
	Vector temp(sw, (sw/a));
	Vector2d ss(temp);
	screen = ss;
}
