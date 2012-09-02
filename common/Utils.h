//
//  Utils.h
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 14/07/12.
//  
//

#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <fstream>

#define DEGREES_TO_RADIANS(__ANGLE__) ((__ANGLE__) / 180.0 * M_PI)

std::string loadFile(std::string _pathFile);

// get number rand
template <class type> type getRand(type _value) {
	return (type)rand()/(type)RAND_MAX * _value;
}

#endif
