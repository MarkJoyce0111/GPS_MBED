#include "pointInPolyCalculator.h"
#include "mbed.h"


//pointInPolyCalculator::pointInPolyCalculator();
pointInPolyCalculator::pointInPolyCalculator(){};

//All posistion variables must be in decimal degrees.
//polyX and polyY are arrays and polySides is thier length (ie the number of sides of the polygon, this is the coords of the poly itself, top left then clockwise around)
//DeciDegX and DeciDegY is the current GPS location to check. 
bool pointInPolyCalculator::pointInPolygon(float *deciDegX, float *deciDegY, int* polySides, float *polyX, float *polyY)
{
    i = *(polySides) - 1;
    j = *(polySides) - 1;
    inside = false;

    //Where the magic happens. Point in polygon. 
    for (i = 0; i < *(polySides); j = i++) {
        if (((*(polyX+i) > *deciDegX) != (*(polyX+j) > *deciDegX)) && (*deciDegY < (*(polyY+j) - *(polyY+i)) * (*deciDegX - *(polyX+i)) / (*(polyX+j) - *(polyX+i)) + *(polyY+i))) {
            inside = !inside;
        }
    }
    return inside;
}
