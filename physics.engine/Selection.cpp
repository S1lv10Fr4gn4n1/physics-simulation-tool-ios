//
//  Selection.cpp
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani da Silva on 03/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "Selection.h"

bool inBBox(SimulatedObject * _simulatedObject, Pointer * _pointer)
{   
    if (_pointer->x < _simulatedObject->getBBox()->max->x &&
        _pointer->x > _simulatedObject->getBBox()->min->x &&
        _pointer->y < _simulatedObject->getBBox()->max->y &&
        _pointer->y > _simulatedObject->getBBox()->min->y) {
        return true;
    }
        
    return false;
}

bool inSimulatedObject(SimulatedObject * _simulatedObject, Pointer * _pointer)
{	    
	int countObjectsRight = 0;
	unsigned long next = 0;
	float ti = 0;
	float xi = 0;
	Pointer * p1 = 0;
	Pointer * p2 = 0;
    
    for (int i=0; i<_simulatedObject->getPointersAux()->size(); i++) {
		p1 = _simulatedObject->getPointersAux()->at(i);
		
		next = (i + 1) % _simulatedObject->getPointersAux()->size();
		
		p2 = _simulatedObject->getPointersAux()->at(next);
		
		// equacoes parametrica da reta
		ti = (_pointer->y - p1->y) / (p2->y - p1->y);
		xi = p1->x + (p2->x - p1->x) * ti;
		
		if ((ti >= 0.0f && ti <= 1.0f) && xi < _pointer->x) {
			countObjectsRight++;
		}
    }
    
    // if there is an odd number of lines crossing the scanline to 
    // the right of the selected point, so that selecting an object.
	if (countObjectsRight % 2 != 0) {
		return true;
	}
	
	return false;
}

SimulatedObject * Selection::selectSimulatedObject(World * _world, Pointer * _pointer)
{
    SimulatedObject * simulatedObject = 0;
    
    for (int i=0; i<_world->getSimulatedObjects()->size(); i++) {
        simulatedObject = _world->getSimulatedObjects()->at(i);

        if (inBBox(simulatedObject, _pointer) && inSimulatedObject(simulatedObject, _pointer)) {
            return simulatedObject;
        }
    }
    
    return 0;
}