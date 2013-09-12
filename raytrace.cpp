// Gina Guerrero
// CpSc 405 - Spring 2011
// Assignment #4
// compile with makefile then run as ./raytrace [l|v] [pixwidth] scene.txt [image-filename.ext]

#include <Magick++.h>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <cstring>
#include <cmath>
#include "Matrix.h"
#include "Sphere.h"
#include "Collision.h"
#include "Material.h"
#include "Light.h"
#include "camera.h"
#include "Color.h"

#define MAXLEVEL 8

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

using namespace std;
using namespace Magick;

// color pixel info
typedef struct RGBAPixel {
	unsigned char r, g, b, a;
} RGBAPixel;


/*********************** GLOBAL VARIABLES ********************************/
int N = 250;
int M;
static int icolor = 0;	
COLOR_C black(0.0,0.0,0.0);
RGBAPixel **pixmap;
Camera camera;
SPHERE_T *sphere;
Light *lights;
int numLi, numSp;
string outfilename = "none";
char *scenefile;

/********************* MAGICK++/OpenGL FUNCTIONS *************************/
/*
  Routine to write the current framebuffer to an image file
*/
void writeimage(){
  N = glutGet(GLUT_WINDOW_WIDTH);
  M = glutGet(GLUT_WINDOW_HEIGHT);
  unsigned char pixmap[4 * N * M];

  glReadPixels(0, 0, N, M, GL_RGBA, GL_UNSIGNED_BYTE, pixmap);
  Image image(N, M, "RGBA", CharPixel, (void *)pixmap);

  image.flip();
  image.write(outfilename);
}
void drawSquare(){
  glutReshapeWindow(N, M);
  glClear(GL_COLOR_BUFFER_BIT);  // clear window to background color
  glRasterPos2i(0, 0);
  glDrawPixels(N, M, GL_RGBA, GL_UNSIGNED_BYTE, pixmap[0]);

  // flush the OpenGL pipeline to the viewport
  //glFlush();
  glutSwapBuffers();
}
/*
 Reshape Callback Routine: sets up the viewport and drawing coordinates
 This routine is called when the window is created and every time the window 
 is resized, by the program or by the user
*/
void handleReshape(int N, int M){
  // make the viewport the entire window
  glViewport(0, 0, N, M);
  
  // define the drawing coordinate system on the viewport
  // to be measured in pixels
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, N, 0, M);
  glMatrixMode(GL_MODELVIEW);
}
/*
  Keyboard Callback Routine: 
  This routine is called every time a key is pressed on the keyboard
*/
void handleKey(unsigned char key, int x, int y){
  
  switch(key){
    case 'w':
    case 'W':
	  if (outfilename != "none") 
		writeimage();
      break;
      
    case 'q':		// q - quit
    case 'Q':
    case 27:		// esc - quit
      exit(0);
      
    default:		// not a valid key -- just ignore it
      return;
  }
}
/***************** END OF MAGICK++/OpenGL FUNCTIONS ***********************/

// tests the command line arguments
int testCommands(int len, char** com) {
	int proj = 1;
	string value;
	
	for(int i = 1; i < len; i++) {
	value = com[i];
	
		if (!value.compare("l")) { 	// parallel viewing
			proj = 0;
		} else if (!value.compare("v")) { 	// perspective viewing
			proj = 1;
		} else if (isdigit(value[0])) {	// width size
			N = atoi(com[i]);
		} else if ((value.find(".txt") != string::npos)) { // scene.txt
			scenefile = new char[value.size()+1];
			strcpy(scenefile, value.c_str());
		} else {
			outfilename = value;	// outfile name
		}
	}
	
	if (scenefile == NULL) {
		cerr << "Are you sure you provided a scene file? ):  Exiting..." << endl;
		exit(1);
	}
	
	return proj;
}

// reads in the provided scene file and stores the data appropriately
void readScene() {
	ifstream indata;
	string check;
	int numof;
	double vars[18];
	int i, k;
	
	indata.open(scenefile);
	if(!indata) {
		cerr << "Could not open the scene file.  Is it in the same directory?" << endl;
		exit(1);
	}
	
	while(indata >> check) {
		
		if (!check.compare("camera")) {
			for(int i = 0; i < 12; i++) {
				indata >> vars[i];
			}
			
			camera.set(new Vector(vars[0], vars[1], vars[2]),
					   new Vector(vars[3], vars[4], vars[5]),
					   new Vector(vars[6], vars[7], vars[8]),
					   vars[9], vars[10], vars[11]);
			
		} else if (!check.compare("lights")) {
			indata >> numof;
			numLi = numof;
			lights = new Light[numof];
			
			for(k = 0; k < numof; k++) {
				for(i = 0; i < 7; i++) {
					indata >> vars[i];
				}
				
				lights[k].set(vars[0], 
							  new Vector(vars[1], vars[2], vars[3]),
							  new COLOR_C(vars[4], vars[5], vars[6]));
			}
			
		} else if (!check.compare("spheres")) {
			indata >> numof;
			numSp = numof;
			sphere = new SPHERE_T[numof];
			
			for(k = 0; k < numof; k++) {
				for(i = 0; i < 18; i++) {
					indata >> vars[i];
				}
				
				sphere[k].set(new Vector(vars[0], vars[1], vars[2]), vars[3], 
							   new Material(new COLOR_C(vars[4], vars[5], vars[6]), 
							   new COLOR_C(vars[7], vars[8], vars[9]),
							   new COLOR_C(vars[10], vars[11], vars[12]),
							   new COLOR_C(vars[13], vars[14], vars[15]), vars[16], vars[17]));
			}
		}
	}	
	
	indata.close();
}

// regular shading (checked == okay)
COLOR_C shader(Collision coll, Material matt, Vector3d ur, Light lit) {
	Vector3d theLight;				
	COLOR_C id, is, lightxmat, finalIntensity;
	Vector3d specLight, viewVec;
	double diff, spec;
	double kd = 0.6;
	double ka = 0.2;
	double ks = 1.0;
	
	// fix the ray info accordingly 
	if(lit.type == 0) {
		theLight = lit.info.normalize();
	} else if (lit.type == 1) {
		theLight = (coll.hitpoint - lit.info).normalize();
	} else {
		theLight = -lit.info;
	}
	
	// ambient lighting
	lightxmat.set(matt.color * matt.ca);
	finalIntensity.set(ka * lightxmat);
	
	// do diffuse lighting
	lightxmat.set((matt.color * matt.cd) * lit.color);
	
	if ((diff = theLight * coll.normal) < 0.0) {
		id.set(-diff * lightxmat);
		finalIntensity.set(finalIntensity + (kd * id));
	}
	
	// do the specular lighting
	lightxmat.set(matt.cs * lit.color);
	specLight = theLight - 2 * (theLight * coll.normal) * coll.normal;
	viewVec = -ur.normalize();
	
	if ((spec = specLight * viewVec) > 0.0) {
		is.set((pow(spec, matt.n)) * lightxmat);
		finalIntensity.set(finalIntensity + (ks * is));
	}
	
	return finalIntensity;
}

// recursive shading
COLOR_C recShader(int obj, Collision collided, Material matter, Vector3d ur, int level) {
	int k,j, blocked;
	int objSp = -1;
	COLOR_C colorTemp, reflectColor;
	RAY p;
	Vector3d um, fixedP, tempVd;
	Vector blank(0.0,0.0,0.0);
	Vector3d blank3d(blank);
	Collision tempC(INFINITY, blank, blank);
	Collision secCollide(INFINITY, blank, blank);
	Material tempM(&black, &black, &black, &black, 0.0, -1.0);
	Light tempL;
	double r0, r, distLight;
	
	if(level > MAXLEVEL) 
		return (black);
		
	um = ur - 2 * (ur * collided.normal) * collided.normal;
	fixedP = collided.hitpoint + (.00001 * collided.normal);
	
	p.set(fixedP, um);
	
	for(k = 0; k < numSp; k++) {
		if(sphere[k].spCheck(p, &tempC) == 1) {
			if(tempC.t < secCollide.t) {
				secCollide.set(tempC);
				tempM.set(sphere[k].material);
				objSp = k;
			}
		}
	}

	// if it hits a sphere
	if(secCollide.t < INFINITY) {
		
		// then recursively call to see if it hits again
		reflectColor.set(recShader(objSp, secCollide, tempM, um, level+1));
		tempL.set(4, um, &reflectColor);
		
		// once it comes back, do the shading
		colorTemp.set(shader(collided, matter, ur, tempL));	
	}
	
	// now does the lighting for all lights NOT blocked at that point
	for(k = 0; k < numLi; k++) {
		blocked = 0;
		tempC.set(INFINITY, blank, blank);
		
		fixedP = collided.hitpoint + (.00001 * collided.normal);
		tempVd = lights[k].info - fixedP;
		p.set(fixedP, tempVd.normalize());
		
		// need to find magnitude of lights[k].info - fixedP;
		distLight = sqrt(tempVd.x * tempVd.x + 
						 tempVd.y * tempVd.y +
						 tempVd.z * tempVd.z);
		
		for(j = 0; j < numSp; j++) {
			if(sphere[j].spCheck(p, &tempC) == 1) {
				if(tempC.t < distLight) {
					blocked = 1;
					break;
				}
			}
		} 
		
		if(blocked == 0) {
			colorTemp.set(colorTemp + shader(collided, matter, ur, lights[k]));
		}
	}
	return colorTemp;
}

/*
   Main program to draw the square, change colors, and wait for quit
*/
int main(int argc, char* argv[]){
	// variable initializations
	int i, j, k, proj, spObj;
	Vector3d c0, ux, uy, uz, pTemp, urTemp;
	double dydi, dxdj, py, px;
	RAY p;
	COLOR_C colorVals;
	Collision tempC, collided;
	Material matter;
	Vector blankV(0.0,0.0,0.0);
	Vector3d blank(blankV);
	
	// takes care of command line arguments & does arbitrary width
	proj = testCommands(argc, argv);
	
	// sets up the camera object
	readScene();
	
	// variable pixel width
	M = (N / camera.aspect);
	
	// sets up the pixmap we'll use
	pixmap = new RGBAPixel*[M];
	pixmap[0] = new RGBAPixel[M*N];
	for(i = 1; i < M; i++) 
		pixmap[i] = pixmap[i-1]+N;
	
	
	if (proj == 0) {
		c0 = camera.vPoint + camera.focal * camera.vDir.normalize();  // screen center
		ux = (camera.vDir.normalize() % camera.vUp).normalize();  	  // horizontal screen direction
		uy = -camera.vDir.normalize() % ux;							  // vertical screen direction
		uz = -camera.vDir.normalize();								  // normal to the viewscreen
	} else {
		c0 = camera.vPoint + camera.focal * camera.vDir;  // screen center
		ux = (camera.vDir % camera.vUp).normalize();  	  // horizontal screen direction
		uy = -camera.vDir % ux;							  // vertical screen direction
		uz = -camera.vDir;								  // normal to the viewscreen
		//uy = uz % ux;
	}
	
	dydi = camera.screen.y / double(M);	// vertical distance between pixels
	dxdj = camera.screen.x / double(N);	// horizontal distance between pixels
	
	for(i = 0; i < M; i++) {	// for each row
		py = - camera.screen.y / 2 + dydi * (i + 0.5);	// y increment of row
		for (j = 0; j < N; j++) {	// for each column
			px = - camera.screen.x / 2 + dxdj * (j + 0.5);	// x increment of row
			pTemp = c0 + px * ux + py * uy;		//pixel center in space
				if (proj == 0)
					urTemp = camera.vDir.normalize();
				else
					urTemp = (pTemp - camera.vPoint).normalize();
			p.set(pTemp, urTemp);  	// information on the ray we're shooting
			
			// resets the material at that point and the collision objects
			matter.set(&black, &black, &black, &black, 0.0, 1.0);
			tempC.set(INFINITY, blank, blank);
			collided.set(INFINITY, blank, blank);
			spObj = -1;
			
			// checks to see if it hit an object
			for(k = 0; k < numSp; k++) {
				if ((sphere[k].spCheck(p, &tempC)) == 1) {
					if (tempC.t < collided.t) {
						collided.set(tempC);
						matter.set(sphere[k].material);
						spObj = k;
					}
				}
			}
			
			// do shading here...send in vars: material and collided
			if(collided.t < INFINITY) {
				// call the recursive ray tracer here..
				colorVals.set(recShader(spObj, collided, matter, urTemp, 1));

			} else {
				colorVals.set(matter.color);
			}
			
			if (colorVals.r > 1.0) { colorVals.r = 1.0; }
			if (colorVals.g > 1.0) { colorVals.g = 1.0; }
			if (colorVals.b > 1.0) { colorVals.b = 1.0; }
			
			pixmap[i][j].r = (unsigned char) (colorVals.r * 255);
			pixmap[i][j].g = (unsigned char) (colorVals.g * 255);
			pixmap[i][j].b = (unsigned char) (colorVals.b * 255);
			pixmap[i][j].a = (unsigned char) (255);
			
		}
	}
	

  
  // start up the glut utilities
  glutInit(&argc, argv);
  
  // create the graphics window, giving width, height, and title text
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowSize(N, M);
  glutCreateWindow("Assignment #4 - CpSc405 - GGUERRE");
  
  // set up the callback routines to be called when glutMainLoop() detects
  // an event
  glutDisplayFunc(drawSquare);	  // display callback
  glutKeyboardFunc(handleKey);	  // keyboard callback
  glutReshapeFunc(handleReshape); // window resize callback

  // define the drawing coordinate system on the viewport
  // lower left is (0, 0), upper right is (WIDTH, HEIGHT)
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, N, 0, M);
  glMatrixMode(GL_MODELVIEW);

  // specify window clear (background) color to be opaque white
  glClearColor(0, 0, 0, 1);

  // Routine that loops forever looking for events. It calls the registered 
  // callback routine to handle each event that is detected
  glutMainLoop();
  
  return 0;
}
