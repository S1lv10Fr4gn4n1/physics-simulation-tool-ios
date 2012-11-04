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

std::string loadFile(std::string _pathFile);

// get number rand
template <class type> type getRand(type _value)
{
	return (type)rand()/(type)RAND_MAX * _value;
}

template <class type> type getRand(type _valueMin, type _valueMax)
{
    type scaled = (type)rand()/RAND_MAX;
    return (_valueMax - _valueMin +1)*scaled + _valueMin;
}

#endif
