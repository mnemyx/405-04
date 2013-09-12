#include "Vector.h"

using namespace std;

#ifndef COLOR_H
#define COLOR_H

using namespace std;

class COLOR_C
{
public: 
    float r, g, b;
    
    COLOR_C();
    COLOR_C(double, double, double);
    void set(double, double, double);
    void set(COLOR_C);
    
	friend COLOR_C operator*(const COLOR_C& c1, const COLOR_C& c2);
	friend COLOR_C operator*(double v, const COLOR_C& c);
	friend COLOR_C operator*(const COLOR_C& c, double v); 
	friend COLOR_C operator+(const COLOR_C& c1, const COLOR_C& c2);
};

#endif
