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
#include "LibraryCollision.h"
#include "LibraryPhysics.h"
#include "Selection.h"
#include "NDC.h"
#include <vector>

class MainEngine {
private:
    World * world;
    
    MainPhysics * mainPhysics;
    MainCollision * mainCollision;
    
    bool running;
    
    NDC * ndc;
protected:
public:
    MainEngine();
    ~MainEngine();

    void start();
    void stop();
    bool isRunning();
    
    void updateInformation(real _duration);
    
    // WORLD
    World * getWorld();
    void zoom(real _scale);
    void pan(real _scaleX, real _scaleY);
    void rotatedScreen(real _width, real _height);
    
    // SIMULATED OBJECT
    void scaleSimulatedObject(SimulatedObject * _simulatedObject, real _scale);
    void rotateSimulatedObject(SimulatedObject * _simulatedObject, real _radians);
    void translateSimulatedObject(SimulatedObject * _simulatedObject, Vector3 * _vector);

    void updatePositionSimulatedObject(SimulatedObject * _simulatedObject, Vector3 * _vector);
    
    void deleteAllSimulatedObjects();
    void deleteSimulatedObject(SimulatedObject * _simulatedObject);
    void makeSimulatedObject(SimulatedObject * _simulatedObject, TypeObject typeObject);
    
    SimulatedObject * selectedSimulatedObject(Vector3 * _vector);
};

#endif
