#ifndef POINTINPOLYCALCULATOR_H
#define POINTINPOLYCALCULATOR_H

#include "mbed.h"

class pointInPolyCalculator
{
    
    public:
    //pointInPolyCalculator();
    pointInPolyCalculator();
    //Prototypes
    bool pointInPolygon(float *, float *, int *, float *, float *);
    
    private:
    int i;
    int j;
    bool inside;
    
};
#endif
