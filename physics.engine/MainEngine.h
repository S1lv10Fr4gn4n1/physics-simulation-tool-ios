//
//  MainEngine.h
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani da Silva on 09/06/12.
//  
//

#ifndef MAINENGINE_H
#define MAINENGINE_H

#include "LibraryCollision.h"
#include "LibraryPhysics.h"
#include "Selection.h"
#include "NDC.h"

class MainEngine {
private:
    World * world;
    
    MainPhysics * mainPhysics;
    MainCollision * mainCollision;
    
    bool running;
    NDC * ndc;
    
    real eyeX;
    real eyeY;
    real eyeZ;
    real centerX;
    real centerY;
    real centerZ;
    
    std::vector<Vector3 *> * createSphere();
    std::vector<Vector3 *> * createPlan();
    std::vector<Vector3 *> * createBox();
    std::vector<Vector3 *> * createTriangleWithTriangularBase();
    std::vector<Vector3 *> * createTriangleWithSquareBase();
protected:
public:
    MainEngine();
    ~MainEngine();

    void start();
    void stop();
    bool isRunning();
    
    void updateInformation(real _duration);
    
    World * getWorld();
    void zoom(real _scale);
    void pan(real _scaleX, real _scaleY);
    void rotatedScreen(real _width, real _height);
    
    void scaleSimulatedObject(SimulatedObject * _simulatedObject, real _scale);
    void rotateSimulatedObject(SimulatedObject * _simulatedObject, real _radians);
    void translateSimulatedObject(SimulatedObject * _simulatedObject, Vector3 * _vector);

    void updatePositionSimulatedObject(SimulatedObject * _simulatedObject, Vector3 * _vector);
    
    void deleteAllSimulatedObjects();
    void deleteSimulatedObject(SimulatedObject * _simulatedObject);
    void makeSimulatedObject2D(TypeObject _typeObject);
    void makeSimulatedObject3D(TypeObject _typeObject);
    
    SimulatedObject * selectedSimulatedObject(Vector3 * _vector);
};

#endif
