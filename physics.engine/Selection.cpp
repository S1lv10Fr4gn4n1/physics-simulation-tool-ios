//
//  Selection.cpp
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani da Silva on 03/07/12.
//  
//

#include "Selection.h"

bool inBBox(SimulatedObject * _simulatedObject, const Vector3 &_vector)
{
//    _simulatedObject->initBBox(_simulatedObject->getMatrixTransformation().data);
//
//    bool result = false;
//    
//    if (_vector.x < _simulatedObject->getBBox()->max.x &&
//        _vector.x > _simulatedObject->getBBox()->min.x &&
//        _vector.y < _simulatedObject->getBBox()->max.y &&
//        _vector.y > _simulatedObject->getBBox()->min.y) {
//        result = true;
//    }
//    
////    _simulatedObject->initBBox(MatrixMakeIdentity());

//    return result;
    return false;
}

bool inSimulatedObject(SimulatedObject * _simulatedObject, Vector3 * _vector)
{
    int countObjectsRight = 0;
    unsigned long next = 0;
    real ti = 0.0f;
    real xi = 0.0f;
    Vector3 p1;
    Vector3 p2;
    Vector3 p1Aux;
    Vector3 p2Aux;
    
    for (int i=0; i<_simulatedObject->getVectorsAux()->size(); i++) {
		p1Aux = _simulatedObject->getVectorsAux()->at(i);
        p1 = MatrixTransformPoint(_simulatedObject->getMatrixTransformation().data, p1Aux);
		
		next = (i + 1) % _simulatedObject->getVectorsAux()->size();
		
		p2Aux = _simulatedObject->getVectorsAux()->at(next);
        p2 = MatrixTransformPoint(_simulatedObject->getMatrixTransformation().data, p2Aux);
		
		// parametric equations of the line
		ti = (_vector->y - p1.y) / (p2.y - p1.y);
		xi = p1.x + (p2.x - p1.x) * ti;
		
		if ((ti >= 0.0f && ti <= 1.0f) && xi < _vector->x) {
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

SimulatedObject * Selection::selectSimulatedObject(World * _world, const Vector3 &_vector)
{
    SimulatedObject * simulatedObject = NULL;
    
    for (int i=0; i<_world->getSimulatedObjects()->size(); i++) {
        simulatedObject = _world->getSimulatedObjects()->at(i);

#if defined (_3D_)
        if (inBBox(simulatedObject, _vector)) {
            return simulatedObject;
        }
#else
        if (inBBox(simulatedObject, _vector) && inSimulatedObject(simulatedObject, _vector)) {
            return simulatedObject;
        }
#endif
    }
    
    return NULL;
}
