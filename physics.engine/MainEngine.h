//
//  MainEngine.h
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani da Silva on 09/06/12.
//  
//

#ifndef MAINENGINE_H
#define MAINENGINE_H

#include "Commons.h"
#include "Selection.h"
#include "NDC.h"
#include <vector>

class MainEngine {
private:
    World * world;
    bool running;
    
    NDC * ndc;
protected:
public:
    MainEngine();
    ~MainEngine();

    void start();
    void stop();
    bool isRunning();
    
    void updateInformation();
    
    // WORLD
    World * getWorld();
    void zoom(float _scale);
    void pan(Pointer * _pointer);
    void centralizedWorld();
    void rotatedScreen(float _width, float _height);
    
    // SIMULATED OBJECT
    void scaleSimulatedObject(SimulatedObject * _simulatedObject, float _scale);
    void rotateSimulatedObject(SimulatedObject * _simulatedObject, float _radians);
    void translateSimulatedObject(SimulatedObject * _simulatedObject, Pointer * _pointer);

    void deleteAllSimulatedObjects();
    void deleteSimulatedObject(SimulatedObject * _simulatedObject);
    void makeSimulatedObject(SimulatedObject * _simulatedObject, TypeObject typeObject);
    
    SimulatedObject * selectedSimulatedObject(Pointer * _pointer);
};

#endif
