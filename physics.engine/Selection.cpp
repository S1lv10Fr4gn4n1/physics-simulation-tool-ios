//
//  Selection.cpp
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani da Silva on 03/07/12.
//  
//

#include "Selection.h"

int countObjectsRight = 0;
unsigned long next = 0;
real ti = 0.0f;
real xi = 0.0f;
Vector3 * p1 = NULL;
Vector3 * p2 = NULL;
Vector3 * p1Aux = NULL;
Vector3 * p2Aux = NULL;


bool inBBox(SimulatedObject * _simulatedObject, Vector3 * _vector)
{
    _simulatedObject->initBBox(_simulatedObject->getMatrixTransformation());

    bool result = false;
    
    if (_vector->x < _simulatedObject->getBBox()->max->x &&
        _vector->x > _simulatedObject->getBBox()->min->x &&
        _vector->y < _simulatedObject->getBBox()->max->y &&
        _vector->y > _simulatedObject->getBBox()->min->y) {
        result = true;
    }
    
    //_simulatedObject->initBBox(MATRIX_IDENTITY);
    
    return result;
}

bool inSimulatedObject(SimulatedObject * _simulatedObject, Vector3 * _vector)
{
    countObjectsRight = 0;
	long next = 0;
	ti = 0.0f;
	xi = 0.0f;
	p1 = NULL;
	p2 = NULL;
	p1Aux = NULL;
	p2Aux = NULL;

    for (int i=0; i<_simulatedObject->getVectorsAux()->size(); i++) {
		p1Aux = _simulatedObject->getVectorsAux()->at(i);
        p1 = MatrixTransformPoint(_simulatedObject->getMatrixTransformation(), p1Aux);
		
		next = (i + 1) % _simulatedObject->getVectorsAux()->size();
		
		p2Aux = _simulatedObject->getVectorsAux()->at(next);
        p2 = MatrixTransformPoint(_simulatedObject->getMatrixTransformation(), p2Aux);
		
		// parametric equations of the line
		ti = (_vector->y - p1->y) / (p2->y - p1->y);
		xi = p1->x + (p2->x - p1->x) * ti;
		
		if ((ti >= 0.0f && ti <= 1.0f) && xi < _vector->x) {
			countObjectsRight++;
		}
        
        delete p1;
        delete p2;
        p1 = NULL;
        p2 = NULL;
    }
    
    // if there is an odd number of lines crossing the scanline to 
    // the right of the selected point, so that selecting an object.
	if (countObjectsRight % 2 != 0) {
		return true;
	}
	
	return false;
}

SimulatedObject * Selection::selectSimulatedObject(World * _world, Vector3 * _vector)
{
    SimulatedObject * simulatedObject = NULL;
    
    for (int i=0; i<_world->getSimulatedObjects()->size(); i++) {
        simulatedObject = _world->getSimulatedObjects()->at(i);

        if (inBBox(simulatedObject, _vector) && inSimulatedObject(simulatedObject, _vector)) {
            return simulatedObject;
        }
    }
    
    return NULL;
}