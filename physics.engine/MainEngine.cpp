//
//  MainEngine.cpp
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani da Silva on 09/06/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "MainEngine.h"

using namespace std;

MainEngine::MainEngine()
{
    this->running = false;

    this->world = new World();
    this->mainPhysics = new MainPhysics();
    this->mainCollision = new MainCollision(USE_TREE);
    this->ndc = new NDC();
    
    this->eyeX = 0.0f;
    this->eyeY = 1.0f;
    this->eyeZ = 2.0f;
    
    this->centerX = 0.0f;
    this->centerY = 0.0f;
    this->centerZ = 0.0f;
}

MainEngine::~MainEngine()
{
    if (this->world) {
        delete this->world;
    }
    if (this->ndc) {
        delete this->ndc;
    }
    if (this->mainCollision) {
        delete this->mainCollision;
    }
    if (this->mainPhysics) {
        delete this->mainPhysics;
    }
    
    this->world = NULL;
    this->ndc = NULL;
    this->mainPhysics = NULL;
    this->mainCollision = NULL;
}

void MainEngine::start()
{
    this->mainCollision->cleanCollisions();

    SimulatedObject * object = NULL;
    for (int i=0; i<this->world->getSimulatedObjects()->size(); i++) {
        object = this->world->getSimulatedObjects()->at(i);
        this->mainCollision->insertObject(object);
    }
    this->running = true;
}

void MainEngine::stop()
{
    this->running = false;
}

bool MainEngine::isRunning()
{
    return this->running;
}

void MainEngine::updateInformation(real _duration)
{
    if (!this->running) {
        return;
    }

    if (_duration <= 0.0f) {
        return;
    }
    
    SimulatedObject * object = NULL;

    // update all forces in all objects
    ForceRegistry::getInstance()->updateForces(_duration);
    
    for (int i=0; i<this->world->getSimulatedObjects()->size(); i++) {
        object = this->world->getSimulatedObjects()->at(i);
        
        // updates the physical features
        this->mainPhysics->updateFeatures(object, _duration);

        // update the object in coarse collision
        this->mainCollision->updateObject(object, _duration);

        object->setMatrixTransformation(object->getGLTransform());
        
        // remove objects that left the scene
        // TODO revise values
        if (real_abs(object->getPosition().x) >= 3.5f ||
            real_abs(object->getPosition().y) >= 4.0f ||
            real_abs(object->getPosition().z) >= 3.5f) {
            this->deleteSimulatedObject(object);
        }
    }
    
    // update collisions
    this->mainCollision->updateContacts(this->world->getSimulatedObjects(), _duration);

    // clean all collisions in quadtree
    this->mainCollision->cleanCollisions();

    object = NULL;
}

void MainEngine::rotatedScreen(real _width, real _height)
{
    this->ndc->update(_width, _height);

#if defined (_3D_)

    this->world->setPerspectiveMatrix(MatrixMakePerspective(DEGREES_TO_RADIANS(60.0f), this->ndc->getAspect(), 0.5f, 10.0f));
    this->world->setLookAtMatrix(MatrixMakeLookAt(this->eyeX, this->eyeY, this->eyeZ,
                                                  this->centerX, this->centerY, this->centerZ,
                                                  0.0f, 1.0f, 0.0f));
#else
    MatrixOrtho(this->world->getOrthoMatrix(), -this->ndc->getAspect(), this->ndc->getAspect(), -1, 1, -1, 1);
#endif
}

void MainEngine::rotateCamera(real _radians)
{
    static int ang = 0;
    const static float radius = 2;

    this->eyeX = (radius * cos(M_PI * ang / 180.0f));
    this->centerX = -this->eyeX;
    this->eyeZ = (radius * sin(M_PI * ang / 180.0f));
    this->centerZ = -this->eyeZ;

    if (_radians < 0) {
        ang-=1;
    } else {
        ang+=1;
    }
    
    this->world->setLookAtMatrix(MatrixMakeLookAt(this->eyeX, this->eyeY, this->eyeZ,
                                                  this->centerX, this->centerY, this->centerZ,
                                                  0.0f, 1.0f, 0.0f));
}

void MainEngine::resetCamera()
{
    this->eyeX = 0.0f;
    this->eyeY = 1.0f;
    this->eyeZ = 2.0f;

    this->centerX = 0.0f;
    this->centerY = 0.0f;
    this->centerZ = 0.0f;

    this->world->setLookAtMatrix(MatrixMakeLookAt(this->eyeX, this->eyeY, this->eyeZ,
                                                  this->centerX, this->centerY, this->centerZ,
                                                  0.0f, 1.0f, 0.0f));
}

void MainEngine::zoom(real _scale)
{
    real value = this->ndc->getAspect() * _scale;

    this->ndc->setLeft(-value);
    this->ndc->setRight(value);
    this->ndc->setBottom(-_scale);
    this->ndc->setTop(_scale);
    
#if defined (_3D_)
    this->eyeZ = _scale;
    
    this->world->setLookAtMatrix(MatrixMakeLookAt(this->eyeX, this->eyeY, this->eyeZ,
                                                  this->centerX, this->centerY, this->centerZ,
                                                  0.0f, 1.0f, 0.0f));
#else
    MatrixOrtho(this->world->getOrthoMatrix(),
                this->ndc->getLeft(),
                this->ndc->getRight(),
                this->ndc->getBottom(),
                this->ndc->getTop(),
                -1,
                1);
#endif
}

void MainEngine::pan(real _scaleX, real _scaleY)
{
    //TODO revise
    this->ndc->setLeft(-this->ndc->getAspect() - _scaleX);
    this->ndc->setRight(this->ndc->getAspect() - _scaleX);
    this->ndc->setBottom(-1 - _scaleY);
    this->ndc->setTop(1 - _scaleY);
    
    this->eyeX = _scaleX;
    this->eyeY = _scaleY;
    
#if defined (_3D_)
    this->world->setLookAtMatrix(MatrixMakeLookAt(this->eyeX, this->eyeY, this->eyeZ,
                                                  this->centerX, this->centerY, this->centerZ,
                                                  0.0f, 1.0f, 0.0f));
#else
    MatrixOrtho(this->world->getOrthoMatrix(),
                this->ndc->getLeft(),
                this->ndc->getRight(),
                this->ndc->getBottom(),
                this->ndc->getTop(),
                -1,
                1);
#endif
}

void MainEngine::scaleSimulatedObject(SimulatedObject * _simulatedObject, real _scale)
{
    real * matrixScale = MatrixMakeScale(_scale, _scale);
    real * matrix = MatrixMultiply(_simulatedObject->getMatrixTransformation().data, matrixScale);
    Matrix4x4 matrix4x4(matrix);
    _simulatedObject->setMatrixTransformation(matrix4x4);
    
    delete [] matrixScale;
    matrixScale = NULL;
    matrix = NULL;
}

void MainEngine::rotateSimulatedObject(SimulatedObject * _simulatedObject, real _radians)
{
    real * matrixRotation = MatrixMakeZRotation(_radians);
    real * matrix = MatrixMultiply(_simulatedObject->getMatrixTransformation().data, matrixRotation);
    Matrix4x4 matrix4x4(matrix);
    _simulatedObject->setMatrixTransformation(matrix4x4);
    
    delete [] matrixRotation;
    matrixRotation = NULL;
    matrix = NULL;
}

void MainEngine::translateSimulatedObject(SimulatedObject * _simulatedObject, const Vector3 &_vector)
{
    //MatrixTranslate(_simulatedObject->getMatrixTransformation().data, _vector);
    _simulatedObject->setTransformMatrixIndex(3, _vector.x);
    _simulatedObject->setTransformMatrixIndex(7, _vector.y);
    _simulatedObject->setTransformMatrixIndex(11, _vector.z);
    _simulatedObject->setAwake();
}

World * MainEngine::getWorld()
{
    return this->world;
}

SimulatedObject * MainEngine::selectedSimulatedObject(Vector3 &_vector)
{
    this->ndc->calcNDCCoordinates(&_vector.x, &_vector.y);
    return Selection::selectSimulatedObject(this->world, _vector);
}

void MainEngine::deleteAllSimulatedObjects()
{
    this->world->deleteAllSimulatedObject();
}

void MainEngine::deleteSimulatedObject(SimulatedObject * _simulatedObject)
{
    ForceRegistry::getInstance()->removeObject(_simulatedObject);
#ifdef USE_TREE
    this->mainCollision->deleteObject(_simulatedObject);
#endif
    this->world->deleteSimulatedObject(_simulatedObject);
}

SimulatedObject * MainEngine::makeSimulatedObject2D(TypeObject _typeObject)
{
    SimulatedObject * simulatedObject = new SimulatedObject();
    simulatedObject->setPosition(0.0f, 0.0f);
    simulatedObject->setMode(TRIANGLE_FAN);
    simulatedObject->setTypeObject(_typeObject);
    simulatedObject->setLinearDamping(0.99f);
    simulatedObject->setAngularDamping(0.8f);
    simulatedObject->setCanSleep(true);
    simulatedObject->setAwake();
    
    ForceRegistry::getInstance()->add(simulatedObject, new Gravity(Vector3(0.0f, -9.8f), false));
    
    switch (_typeObject) {
        case CIRCLE:
        {
            this->translateSimulatedObject(simulatedObject, simulatedObject->getPosition());
            
            simulatedObject->setRadius(0.052083f);
            simulatedObject->setHalfSize(Vector3(0.052083f, 0.052083f));
            
            // calculates the radius
            // takes the first point, which indicates the origin of the circle
            Vector3 * v1 = new Vector3(0.0f, 0.0f);
            Vector3 * v2 = new Vector3(0.0f, 0.052083f);
            
            real x = v2->x - v1->x;
            real y = v2->y - v1->y;
            
            // d²=(x0-x)²+(y0-y)²
            real d = (x*x) + (y*y);
            real radius = real_pow(d, 0.5f);
            
            real x1;
            real y1;
            
            // generates points to create the circle, these points are stored
            // to be subsequently used in the algorithm scanline
            int ang = 0;
            for (int i=0; i<36; i++) {
                x1 = (radius * cos(M_PI * ang / 180.0f));
                y1 = (radius * sin(M_PI * ang / 180.0f));
                
                simulatedObject->addVector3(Vector3(x1 + v1->x, y1 + v1->y));
                ang+=10;
            }
            
            delete v1;
            delete v2;
            v1 = NULL;
            v2 = NULL;
            
            break;
        }
            
        case SQUARE:
        {
            simulatedObject->setHalfSize(0.052083f, 0.052083f);
            simulatedObject->addVector3(Vector3( -simulatedObject->getHalfSize().x, -simulatedObject->getHalfSize().y));
            simulatedObject->addVector3(Vector3(  simulatedObject->getHalfSize().x, -simulatedObject->getHalfSize().y));
            simulatedObject->addVector3(Vector3(  simulatedObject->getHalfSize().x,  simulatedObject->getHalfSize().y));
            simulatedObject->addVector3(Vector3( -simulatedObject->getHalfSize().x,  simulatedObject->getHalfSize().y));
            break;
        }
            
        case TRIANGLE:
        {
            simulatedObject->setHalfSize(0.052083f, 0.052083f);
            simulatedObject->addVector3(Vector3(  0.000000f,  0.052083f));
            simulatedObject->addVector3(Vector3( -0.052083f, -0.052083f));
            simulatedObject->addVector3(Vector3(  0.052083f, -0.052083f));
            break;
        }
            
        case PLAN:
        {
            // TODO this is max screem possible for simulation
            // left: -3.960000, right: 3.960000, bottom: -2.970000, top: 2.970000
            simulatedObject->setMode(LINE_LOOP);
            simulatedObject->setColorAux(255, 255, 255, 0);
            simulatedObject->setMass(0.0f); // immovable
            simulatedObject->setPosition(0.0f, -0.9f);
            simulatedObject->setHalfSize(Vector3(5.0f, 0.0f));
            simulatedObject->addVector3(Vector3(-5.0f, 0.0f));
            simulatedObject->addVector3(Vector3( 5.0f, 0.0f));
//            simulatedObject->addVector3(new Vector3(-5.0f, -7.0f));
//            simulatedObject->addVector3(new Vector3(-5.0f, -0.9f));
//            simulatedObject->addVector3(new Vector3( 5.0f, -0.9f));
//            simulatedObject->addVector3(new Vector3( 5.0f, -7.0f));
            
            
            break;
        }

        case PARTICLE:
            simulatedObject->addVector3(Vector3(0.0f, 0.5f));
            break;

        default:
            break;
    }
    
    simulatedObject->initialize();
    this->world->addSimulatedObject(simulatedObject);
    
    return simulatedObject;
}

void MainEngine::addAndInitializeSimulatedObject3D(SimulatedObject * _simulatedObject, const Vector3 &_gravity)
{
    ForceRegistry::getInstance()->add(_simulatedObject, new Gravity(_gravity, false));

    _simulatedObject->setAccelerationGravity(_gravity);

    _simulatedObject->initialize();
    this->world->addSimulatedObject(_simulatedObject);
}

SimulatedObject * MainEngine::makeSimulatedObject3D(TypeObject _typeObject)
{
    SimulatedObject * simulatedObject = new SimulatedObject();
    simulatedObject->setTypeObject(_typeObject);
    simulatedObject->setMass(4.0f);
    simulatedObject->setRestitution(0.3f);
    simulatedObject->setPosition(0.0f, 0.1f, 0.0f);
    simulatedObject->setLinearDamping(0.99f);
    simulatedObject->setAngularDamping(0.8f);
    simulatedObject->setCanSleep(true);
    simulatedObject->setAwake();
    simulatedObject->setSelected(true);
    
    
    switch (_typeObject) {
        case SPHERE:
        {
            simulatedObject->setMode(TRIANGLE_STRIP);
            simulatedObject->setRadius(0.1f);
            simulatedObject->addAllVectors(this->createSphere(simulatedObject->getPosition(), simulatedObject->getRadius()));
            
            Matrix3 tensor;
            real coeff = 0.4f*simulatedObject->getMass() * simulatedObject->getRadius()*simulatedObject->getRadius();
            tensor.setInertiaTensorCoeffs(coeff,coeff,coeff);
            simulatedObject->setInertiaTensor(tensor);

            break;
        }
        case BOX:
        {
            simulatedObject->setHalfSize(0.1f, 0.1f, 0.1f);
            simulatedObject->addAllVectors(this->createBox(simulatedObject->getPosition(), simulatedObject->getHalfSize()));

            Matrix3 tensor;
            tensor.setBlockInertiaTensor(simulatedObject->getHalfSize(), simulatedObject->getMass());
            simulatedObject->setInertiaTensor(tensor);

            break;
        }
        case PLAN:
        {
            simulatedObject->setSelected(false);
            simulatedObject->setPosition(0.0f, 0.0f, 0.0f);
            simulatedObject->setMass(0.0f);
            simulatedObject->setFriction(0.9f);
            simulatedObject->setMode(LINES);
            simulatedObject->setColorAux(0, 0, 0, 0);
            simulatedObject->setHalfSize(3.0f, 0.0f, 3.0f);
            simulatedObject->addAllVectors(this->createPlan(simulatedObject->getPosition()));            
            break;
        }
            
        case PYRAMID:
            simulatedObject->setHalfSize(0.05f, 0.05f, 0.05f);
            simulatedObject->addAllVectors(this->createTriangleWithSquareBase(simulatedObject->getPosition(), simulatedObject->getHalfSize()));
            break;

//        case TRIANGLE_TRIANGULAR_BASE:
//            simulatedObject->setHalfSize(0.1f, 0.1f, 0.1f);
//            simulatedObject->addAllVectors(this->createTriangleWithTriangularBase(simulatedObject->getPosition(), simulatedObject->getHalfSize()));
//            break;


        case CONE:
            simulatedObject->setHalfSize(1.01f, 1.01f, 1.01f);
            simulatedObject->addAllVectors(this->createCone(simulatedObject->getPosition(), simulatedObject->getHalfSize()));
            break;
            
        default:
            break;

    }
    
    if (simulatedObject->getMode() == -1) {
        simulatedObject->setMode(TRIANGLES);
    }

    return simulatedObject;
}

std::vector<Vector3> * MainEngine::createSphere(const Vector3 &_origin, real _radius)
{
    // TODO revise: consider _origem
    
    // Based on http://www.swiftless.com/tutorials/opengl/sphere.html
    vector<Vector3> * vectors = new vector<Vector3>();
    
    const real degreeIncrement = 10; // 10 degrees between
    const real M_PI_Divided_By_180 = M_PI/180;
    
    for (real z = 0; z <= 180 - degreeIncrement; z += degreeIncrement) {
        for (real c = 0; c <= 360 - degreeIncrement; c += degreeIncrement) {
            Vector3 vector1;
            vector1.x = _radius * real_sin( (c) * M_PI_Divided_By_180 ) * real_sin( (z) * M_PI_Divided_By_180 );
            vector1.y = _radius * real_cos( (c) * M_PI_Divided_By_180 ) * real_sin( (z) * M_PI_Divided_By_180 );
            vector1.z = _radius * real_cos( (z) * M_PI_Divided_By_180 );
            vectors->push_back(vector1);
            
            Vector3 vector2;
            vector2.x = _radius * real_sin( (c) * M_PI_Divided_By_180 ) * real_sin( (z + degreeIncrement) * M_PI_Divided_By_180 );
            vector2.y = _radius * real_cos( (c) * M_PI_Divided_By_180 ) * real_sin( (z + degreeIncrement) * M_PI_Divided_By_180 );
            vector2.z = _radius * real_cos( (z + degreeIncrement) * M_PI_Divided_By_180 );
            vectors->push_back(vector2);

            Vector3 vector3;
            vector3.x = _radius * real_sin( (c + degreeIncrement) * M_PI_Divided_By_180 ) * real_sin( (z) * M_PI_Divided_By_180 );
            vector3.y = _radius * real_cos( (c + degreeIncrement) * M_PI_Divided_By_180 ) * real_sin( (z) * M_PI_Divided_By_180 );
            vector3.z = _radius * real_cos( (z) * M_PI_Divided_By_180 );
            vectors->push_back(vector3);
            
            Vector3 vector4;
            vector4.x = _radius * real_sin( (c + degreeIncrement) * M_PI_Divided_By_180 ) * real_sin( (z + degreeIncrement) * M_PI_Divided_By_180 );
            vector4.y = _radius * real_cos( (c + degreeIncrement) * M_PI_Divided_By_180 ) * real_sin( (z + degreeIncrement) * M_PI_Divided_By_180 );
            vector4.z = _radius * real_cos( (z + degreeIncrement) * M_PI_Divided_By_180 );
            vectors->push_back(vector4);
        }
    }
    
    return vectors;
}

vector<Vector3> * MainEngine::createPlan(const Vector3 &_origin)
{
    // TODO revise: consider _origem
    
    real x = 3.0f;
    real y = 0.0f;
    real z = 3.0f;
    real inc = 0.0f;
    int lines = 31;

    vector<Vector3> * vectors = new vector<Vector3>();
    
    for (int i=0; i<lines; i++) {
        vectors->push_back(Vector3(x - inc, y, z));
        vectors->push_back(Vector3(x - inc, y, -z));
        
        inc+=0.2;
    }
    
    inc = 0;
    for (int i=0; i<lines; i++) {
        vectors->push_back(Vector3(x, y, z - inc));
        vectors->push_back(Vector3(-x, y, z - inc));
        
        inc+=0.2;
    }
    
    return vectors;
}

vector<Vector3> * MainEngine::createBox(const Vector3 &_origin, const Vector3 &_halfSize)
{
    // TODO revise: consider _origem and _halfSize
    vector<Vector3> * vectors = new vector<Vector3>();
    vectors->push_back(Vector3(0.1f, -0.1f, -0.1f));
    vectors->push_back(Vector3(0.1f, 0.1f, -0.1f));
    vectors->push_back(Vector3(0.1f, -0.1f, 0.1f));
    vectors->push_back(Vector3(0.1f, -0.1f, 0.1f));
    vectors->push_back(Vector3(0.1f, 0.1f, -0.1f));
    vectors->push_back(Vector3(0.1f, 0.1f, 0.1f));
    
    vectors->push_back(Vector3(0.1f, 0.1f, -0.1f));
    vectors->push_back(Vector3(-0.1f, 0.1f, -0.1f));
    vectors->push_back(Vector3(0.1f, 0.1f, 0.1f));
    vectors->push_back(Vector3(0.1f, 0.1f, 0.1f));
    vectors->push_back(Vector3(-0.1f, 0.1f, -0.1f));
    vectors->push_back(Vector3(-0.1f, 0.1f, 0.1f));
    
    vectors->push_back(Vector3(-0.1f, 0.1f, -0.1f));
    vectors->push_back(Vector3(-0.1f, -0.1f, -0.1f));
    vectors->push_back(Vector3(-0.1f, 0.1f, 0.1f));
    vectors->push_back(Vector3(-0.1f, 0.1f, 0.1f));
    vectors->push_back(Vector3(-0.1f, -0.1f, -0.1f));
    vectors->push_back(Vector3(-0.1f, -0.1f, 0.1f));
    
    vectors->push_back(Vector3(-0.1f, -0.1f, -0.1f));
    vectors->push_back(Vector3(0.1f, -0.1f, -0.1f));
    vectors->push_back(Vector3(-0.1f, -0.1f, 0.1f));
    vectors->push_back(Vector3(-0.1f, -0.1f, 0.1f));
    vectors->push_back(Vector3(0.1f, -0.1f, -0.1f));
    vectors->push_back(Vector3(0.1f, -0.1f, 0.1f));
    
    vectors->push_back(Vector3(0.1f, 0.1f, 0.1f));
    vectors->push_back(Vector3(-0.1f, 0.1f, 0.1f));
    vectors->push_back(Vector3(0.1f, -0.1f, 0.1f));
    vectors->push_back(Vector3(0.1f, -0.1f, 0.1f));
    vectors->push_back(Vector3(-0.1f, 0.1f, 0.1f));
    vectors->push_back(Vector3(-0.1f, -0.1f, 0.1f));
    
    vectors->push_back(Vector3(0.1f, -0.1f, -0.1f));
    vectors->push_back(Vector3(-0.1f, -0.1f, -0.1f));
    vectors->push_back(Vector3(0.1f, 0.1f, -0.1f));
    vectors->push_back(Vector3(0.1f, 0.1f, -0.1f));
    vectors->push_back(Vector3(-0.1f, -0.1f, -0.1f));
    vectors->push_back(Vector3(-0.1f, 0.1f, -0.1f));
    
    return vectors;
}

vector<Vector3> * MainEngine::createTriangleWithTriangularBase(const Vector3 &_origin, const Vector3 &_halfSize)
{
    vector<Vector3> * vectors = new vector<Vector3>();
    vectors->push_back(Vector3(0.1f, 0.0f, 0.0f));
    vectors->push_back(Vector3(-0.1f, 0.0f, 0.0f));
    vectors->push_back(Vector3(0.0f, 0.0f, 0.1f));
    vectors->push_back(Vector3(0.1f, 0.0f, 0.0f));
    
    vectors->push_back(Vector3(0.1f, 0.0f, 0.0f));
    vectors->push_back(Vector3(-0.1f, 0.0f, 0.0f));
    vectors->push_back(Vector3(0.0f, 0.1f, 0.0f));
    vectors->push_back(Vector3(0.1f, 0.0f, 0.0f));
    
    vectors->push_back(Vector3(0.1f, 0.0f, 0.0f));
    vectors->push_back(Vector3(0.0f, 0.0f, 0.1f));
    vectors->push_back(Vector3(0.0f, 0.1f, 0.0f));
    vectors->push_back(Vector3(0.1f, 0.0f, 0.0f));
    
    vectors->push_back(Vector3(0.0f, 0.0f, 0.1f));
    vectors->push_back(Vector3(-0.1f, 0.0f, 0.0f));
    vectors->push_back(Vector3(0.0f, 0.1f, 0.0f));
    vectors->push_back(Vector3(0.0f, 0.0f, 0.1f));

    return vectors;
}

vector<Vector3> * MainEngine::createTriangleWithSquareBase(const Vector3 &_origin, const Vector3 &_halfSize)
{
    // TODO revise: consider _origem and _halfSize
    vector<Vector3> * vectors = new vector<Vector3>();
    vectors->push_back(Vector3(0.05f, 0.0f, -0.05f));
    vectors->push_back(Vector3(0.05f, 0.0f, 0.05f));
    vectors->push_back(Vector3(-0.05f, 0.0f, 0.05f));
    vectors->push_back(Vector3(-0.05f, 0.0f, -0.05f));
    vectors->push_back(Vector3(0.05f, 0.0f, -0.05f));
    
    vectors->push_back(Vector3(0.05f, 0.0f, -0.05f));
    vectors->push_back(Vector3(0.05f, 0.0f, 0.05f));
    vectors->push_back(Vector3(0.0f, 0.1f, 0.0f));
    vectors->push_back(Vector3(0.05f, 0.0f, -0.05f));
    
    vectors->push_back(Vector3(0.05f, 0.0f, 0.05f));
    vectors->push_back(Vector3(-0.05f, 0.0f, 0.05f));
    vectors->push_back(Vector3(0.0f, 0.1f, 0.0f));
    vectors->push_back(Vector3(0.05f, 0.0f, 0.05f));
    
    vectors->push_back(Vector3(-0.05f, 0.0f, 0.05f));
    vectors->push_back(Vector3(-0.05f, 0.0f, -0.05f));
    vectors->push_back(Vector3(0.0f, 0.1f, 0.0f));
    vectors->push_back(Vector3(-0.05f, 0.0f, 0.05f));
    
    vectors->push_back(Vector3(-0.05f, 0.0f, -0.05f));
    vectors->push_back(Vector3(0.05f, 0.0f, -0.05f));
    vectors->push_back(Vector3(0.0f, 0.1f, 0.0f));
    vectors->push_back(Vector3(-0.05f, 0.0f, -0.05f));
                                    
    return vectors;
}

vector<Vector3> * MainEngine::createCone(const Vector3 &_origin, const Vector3 &_halfSize)
{
    // TODO revise: consider _origem and _halfSize
    vector<Vector3> * vectors = new vector<Vector3>();
    vectors->push_back(Vector3(-0.329893f, -0.244687f, -0.965621f));
    vectors->push_back(Vector3(-0.134803f, -0.244687f, -0.946406f));
    vectors->push_back(Vector3(0.052790f, -0.244687f, -0.889500f));
    vectors->push_back(Vector3(0.225677f, -0.244687f, -0.797090f));
    vectors->push_back(Vector3(0.377213f, -0.244687f, -0.672727f));
    vectors->push_back(Vector3(0.501576f, -0.244687f, -0.521191f));
    vectors->push_back(Vector3(0.593986f, -0.244687f, -0.348304f));
    vectors->push_back(Vector3(0.650892f, -0.244687f, -0.160711f));
    vectors->push_back(Vector3(0.670107f, -0.244687f, 0.034379f));
    vectors->push_back(Vector3(-0.329893f, 1.755313f, 0.034379f));
    vectors->push_back(Vector3(0.650892f, -0.244687f, 0.229470f));
    vectors->push_back(Vector3(0.593986f, -0.244687f, 0.417063f));
    vectors->push_back(Vector3(0.501576f, -0.244687f, 0.589949f));
    vectors->push_back(Vector3(0.377213f, -0.244687f, 0.741486f));
    vectors->push_back(Vector3(0.225677f, -0.244687f, 0.865849f));
    vectors->push_back(Vector3(0.052790f, -0.244687f, 0.958259f));
    vectors->push_back(Vector3(-0.134803f, -0.244687f, 1.015165f));
    vectors->push_back(Vector3(-0.329894f, -0.244687f, 1.034379f));
    vectors->push_back(Vector3(-0.524984f, -0.244687f, 1.015164f));
    vectors->push_back(Vector3(-0.712577f, -0.244687f, 0.958259f));
    vectors->push_back(Vector3(-0.885464f, -0.244687f, 0.865849f));
    vectors->push_back(Vector3(-1.037001f, -0.244687f, 0.741486f));
    vectors->push_back(Vector3(-1.161363f, -0.244687f, 0.589949f));
    vectors->push_back(Vector3(-1.253773f, -0.244687f, 0.417062f));
    vectors->push_back(Vector3(-1.310679f, -0.244687f, 0.229469f));
    vectors->push_back(Vector3(-1.329893f, -0.244687f, 0.034378f));
    vectors->push_back(Vector3(-1.310679f, -0.244687f, -0.160712f));
    vectors->push_back(Vector3(-1.253773f, -0.244687f, -0.348305f));
    vectors->push_back(Vector3(-1.161362f, -0.244687f, -0.521192f));
    vectors->push_back(Vector3(-1.036999f, -0.244687f, -0.672728f));
    vectors->push_back(Vector3(-0.885463f, -0.244687f, -0.797091f));
    vectors->push_back(Vector3(-0.712576f, -0.244687f, -0.889501f));
    vectors->push_back(Vector3(-0.524982f, -0.244687f, -0.946406f));
    
    return vectors;
}
