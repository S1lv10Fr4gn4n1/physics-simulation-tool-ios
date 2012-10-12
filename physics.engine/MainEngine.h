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
    bool running;

    World * world;

    SimulatedObject * plan;
    
    MainPhysics * mainPhysics;
    MainCollision * mainCollision;
    NDC * ndc;

    bool checkCollission(Vector3 &winPos, SimulatedObject * object);

    std::vector<Vector3> * createSphere(const Vector3 &_origin, real _radius);
    std::vector<Vector3> * createPlan(const Vector3 &_origin);
    std::vector<Vector3> * createBox(const Vector3 &_origin, const Vector3 &_halfSize);
    std::vector<Vector3> * createTriangleWithTriangularBase(const Vector3 &_origin, const Vector3 &_halfSize);
    std::vector<Vector3> * createTriangleWithSquareBase(const Vector3 &_origin, const Vector3 &_halfSize);
    std::vector<Vector3> * createCone(const Vector3 &_origin, const Vector3 &_halfSize);
protected:
public:
    MainEngine();
    ~MainEngine();

    void start();
    void stop();
    bool isRunning();
    bool isUpdatedBeforeDraw();
    
    void updateInformation(real _duration);
    
    World * getWorld();
    void zoom(real _scale);
    void pan(real _scaleX, real _scaleY);
    void rotatedScreen(real _width, real _height);
    
    void scaleSimulatedObject(SimulatedObject * _simulatedObject, real _scale);
    void rotateSimulatedObject(SimulatedObject * _simulatedObject, real _radians);
    void translateSimulatedObject(SimulatedObject * _simulatedObject, const Vector3 &_vector);

    void rotateCamera(real _radians);
    void resetCamera();

    void deleteAllSimulatedObjects();
    void deleteSimulatedObject(SimulatedObject * _simulatedObject);
    SimulatedObject * makeSimulatedObject2D(TypeObject _typeObject);
    SimulatedObject * makeSimulatedObject3D(TypeObject _typeObject);
    void addAndInitializeSimulatedObject3D(SimulatedObject * _simulatedObject, const Vector3 &_gravity=Vector3(0.0f, -9.8f, 0.0f));
    
    SimulatedObject * selectedSimulatedObject(Vector3 &_vector);

    bool alreadyExistsPlan();
    SimulatedObject * getExistingPlan();
};

#endif
