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
    
    std::vector<Vector3> * createSphere(const Vector3 &_origin, real _radius);
    std::vector<Vector3> * createPlan(const Vector3 &_origin);
    std::vector<Vector3> * createBox(const Vector3 &_origin, const Vector3 &_halfSize);
    std::vector<Vector3> * createTriangleWithTriangularBase();
    std::vector<Vector3> * createTriangleWithSquareBase();
    std::vector<Vector3> * createCone();
    std::vector<Vector3> * paintQuadTree();
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
    SimulatedObject * makeSimulatedObject2D(TypeObject _typeObject);
    SimulatedObject * makeSimulatedObject3D(TypeObject _typeObject, bool _init=true);
    
    SimulatedObject * selectedSimulatedObject(Vector3 * _vector);
};

#endif
