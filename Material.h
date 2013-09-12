#include "Vector.h"
#include "Color.h"

using namespace std;

#ifndef MATERIAL_H
#define MATERIAL_H

using namespace std;

class Material
{
public: 
    COLOR_C color, ca, cd, cs;
    double n, ref;
    
    Material();
    Material(COLOR_C*, COLOR_C*, COLOR_C*, COLOR_C*, double, double);
    void set(COLOR_C*, COLOR_C*, COLOR_C*, COLOR_C*, double, double);
    void set(Material);
    
};

#endif
