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
    this->mainCollision = new MainCollision();
    this->ndc = new NDC();
    
    this->eyeX = 0.0f;
    this->eyeY = 2.0f;
    this->eyeZ = 6.0f;
    
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
    SimulatedObject * object = NULL;
    for (int i=0; i<this->world->getSimulatedObjects()->size(); i++) {
        object = this->world->getSimulatedObjects()->at(i);
        this->mainCollision->updateObject(object, 0.0f);
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

    if (_duration <= 0.0) {
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
        
        // mainEngine(this) translate object
        this->translateSimulatedObject(object, object->getPosition());
        
        // remove objects that left the scene
        if (real_abs(object->getPosition()->x) >= 4.0f ||
            real_abs(object->getPosition()->y) >= 3.0f) {
            this->mainCollision->deleteObject(object);
            this->deleteSimulatedObject(object);
        }
    }
    
    // update collisions
    this->mainCollision->updateContacts(_duration);

    object = NULL;
}

void MainEngine::rotatedScreen(real _width, real _height)
{
    this->ndc->update(_width, _height);

#if defined (_3D_)
    MatrixMakePerspective(this->world->getPerspectiveMatrix(), DEGREES_TO_RADIANS(60.0f), this->ndc->getAspect(), 0.5f, 10.0f);
    MatrixMakeLookAt(this->world->getLookAtMatrix(),
                     this->eyeX, this->eyeY, this->eyeZ,
                     this->centerX, this->centerY, this->centerZ,
                     0.0f, 1.0f, 0.0f);
#else
    MatrixOrtho(this->world->getOrthoMatrix(), -this->ndc->getAspect(), this->ndc->getAspect(), -1, 1, -1, 1);
#endif
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
    
    MatrixMakeLookAt(this->world->getLookAtMatrix(),
                     this->eyeX, this->eyeY, this->eyeZ,
                     this->centerX, this->centerY, this->centerZ,
                     0.0f, 1.0f, 0.0f);
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
    MatrixMakeLookAt(this->world->getLookAtMatrix(),
                     this->eyeX, this->eyeY, this->eyeZ,
                     this->centerX, this->centerY, this->centerZ,
                     0.0f, 1.0f, 0.0f);
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
    real * matrix = MatrixMultiply(_simulatedObject->getMatrixTransformation(), matrixScale);
    _simulatedObject->setMatrixTransformation(matrix);
    
    delete [] matrixScale;
    matrixScale = NULL;
    matrix = NULL;
}

void MainEngine::rotateSimulatedObject(SimulatedObject * _simulatedObject, real _radians)
{
    real * matrixRotation = MatrixMakeZRotation(_radians);
    real * matrix = MatrixMultiply(_simulatedObject->getMatrixTransformation(), matrixRotation);
    _simulatedObject->setMatrixTransformation(matrix);
    
    delete [] matrixRotation;
    matrixRotation = NULL;
    matrix = NULL;
}

void MainEngine::translateSimulatedObject(SimulatedObject * _simulatedObject, Vector3 * _vector)
{
    MatrixTranslate(_simulatedObject->getMatrixTransformation(), _vector);
}

void MainEngine::updatePositionSimulatedObject(SimulatedObject * _simulatedObject, Vector3 * _vector)
{
    Vector3 * position = _simulatedObject->getPosition();
    position->x = _vector->x;
    position->y = _vector->y;
    position->z = _vector->z;
}

World * MainEngine::getWorld()
{
    return this->world;
}

SimulatedObject * MainEngine::selectedSimulatedObject(Vector3 * _vector)
{
    this->ndc->calcNDCCoordinates(&_vector->x, &_vector->y);
    return Selection::selectSimulatedObject(this->world, _vector);
}

void MainEngine::deleteAllSimulatedObjects()
{
    this->world->deleteAllSimulatedObject();
}

void MainEngine::deleteSimulatedObject(SimulatedObject * _simulatedObject)
{
    ForceRegistry::getInstance()->removeObject(_simulatedObject);
    this->mainCollision->deleteObject(_simulatedObject);
    this->world->deleteSimulatedObject(_simulatedObject);
}

void MainEngine::makeSimulatedObject2D(TypeObject _typeObject)
{
    // TODO revise: values of initialization
    SimulatedObject * simulatedObject = new SimulatedObject();
    simulatedObject->setColorAux(Color::MakeRandonColor());
    simulatedObject->setMode(TRIANGLE_FAN);
    simulatedObject->setTypeObject(_typeObject);
    simulatedObject->setRadius(0.052083f);     // TODO revise: how get radius ?
    simulatedObject->setLinearDamping(0.99f);
    simulatedObject->setAngularDamping(0.8f);
//    ForceRegistry::getInstance()->add(object, new Gravity(Vector3::MakeVector3(0.0f, -9.8f)));
//    ForceRegistry::getInstance()->add(object, new Drag(object->getDamping(), object->getDamping()*object->getDamping()));
    
    // TODO revise
    //    cyclone::Matrix3 tensor;
    //    cyclone::real coeff = 0.4f*body->getMass()*radius*radius;
    //    tensor.setInertiaTensorCoeffs(coeff,coeff,coeff);
    //    body->setInertiaTensor(tensor);

#if defined(STRESS_TEST)
    simulatedObject->setVelocity(getRand(2.0f), getRand(3.0f));
    simulatedObject->setLinearDamping(getRand(0.9f));
    simulatedObject->setAngularDamping(getRand(0.9f));
    simulatedObject->setRadius(0.052083f);
#endif
    
    switch (_typeObject) {
        case CIRCLE:
        {
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
                
                simulatedObject->addVector3(new Vector3(x1 + v1->x, y1 + v1->y));
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
            simulatedObject->addVector3(new Vector3( -0.052083f, -0.052083f));
            simulatedObject->addVector3(new Vector3(  0.052083f, -0.052083f));
            simulatedObject->addVector3(new Vector3(  0.052083f,  0.052083f));
            simulatedObject->addVector3(new Vector3( -0.052083f,  0.052083f));
            break;
        }
            
        case TRIANGLE:
        {
            simulatedObject->addVector3(new Vector3(  0.000000f,  0.052083f));
            simulatedObject->addVector3(new Vector3( -0.052083f, -0.052083f));
            simulatedObject->addVector3(new Vector3(  0.052083f, -0.052083f));
            break;
        }
            
        case PLAN:
        {
            // TODO this is max screem possible for simulation
            // left: -3.960000, right: 3.960000, bottom: -2.970000, top: 2.970000
            simulatedObject->setMass(0.0f); // immovable
            simulatedObject->addVector3(new Vector3(-5.0f, -7.0f));
            simulatedObject->addVector3(new Vector3(-5.0f, -0.9f));
            simulatedObject->addVector3(new Vector3( 5.0f, -0.9f));
            simulatedObject->addVector3(new Vector3( 5.0f, -7.0f));
            break;
        }
            
        case ENGINE:
            break;
            
        case SPRING:
            break;
            
        case SPRINGS:
            break;
            
        case POLYGON_OPEN:
            break;
            
        case POLYGON_CLOSE:
            break;
        
        case PARTICLE:
            simulatedObject->addVector3(new Vector3(0.0f, 0.5f));
            break;

        default:
            break;
    }
    
    simulatedObject->initialize();
    this->world->addSimulatedObject(simulatedObject);
}

void MainEngine::makeSimulatedObject3D(TypeObject _typeObject)
{
    // TODO revise: values of initialization
    SimulatedObject * simulatedObject = new SimulatedObject();
    simulatedObject->setTypeObject(_typeObject);
    simulatedObject->setVelocity(getRand(2.0f), getRand(3.0f));
    simulatedObject->setLinearDamping(0.99f);  //getRand(0.9f));
    simulatedObject->setAngularDamping(0.8f);  //getRand(0.9f));

    switch (_typeObject) {
        case SPHERE:
            simulatedObject->addAllVectors(this->createSphere());
            break;

        case BOX:
            simulatedObject->addAllVectors(this->createBox());
            break;
            
        case PLAN:
        {
            simulatedObject->setMass(0.0f);
            simulatedObject->setMode(LINES);
            simulatedObject->setColorAux(Color::MakeColor(0, 0, 0, 0));
            simulatedObject->addAllVectors(this->createPlan());
            
            break;
        }
            
        case TRIANGLE_SQUARE_BASE:
            simulatedObject->addAllVectors(this->createTriangleWithSquareBase());
            break;

        case TRIANGLE_TRIANGULAR_BASE:
            simulatedObject->addAllVectors(this->createTriangleWithTriangularBase());
            break;
            
        default:
            break;

    }
    
    if (!simulatedObject->getColorAux()) {
        simulatedObject->setColorAux(Color::MakeRandonColor());
    }
    
    if (simulatedObject->getMode() == -1) {
        simulatedObject->setMode(TRIANGLES);
    }
    
    if (simulatedObject->getRadius() == 0.0f) {
        simulatedObject->setRadius(0.1f); // TODO revise: how get radius ?
    }

    simulatedObject->initialize();
    this->world->addSimulatedObject(simulatedObject);
}

std::vector<Vector3 *> * MainEngine::createSphere()
{
    // Based on http://www.swiftless.com/tutorials/opengl/sphere.html
    vector<Vector3 *> * vectors = new vector<Vector3 *>();
    
    const real degreeIncrement = 10; // 10 degrees between
    const real M_PI_Divided_By_180 = M_PI/180;
    real radius = 0.1f;
    
    Vector3 * vector = NULL;
    
    for (real z = 0; z <= 180 - degreeIncrement; z += degreeIncrement) {
        for (real c = 0; c <= 360 - degreeIncrement; c += degreeIncrement) {
            vector = new Vector3();
            vector->x = radius * sinf( (c) * M_PI_Divided_By_180 ) * sinf( (z) * M_PI_Divided_By_180 );
            vector->y = radius * cosf( (c) * M_PI_Divided_By_180 ) * sinf( (z) * M_PI_Divided_By_180 );
            vector->z = radius * cosf( (z) * M_PI_Divided_By_180 );
            vectors->push_back(vector);
            
            vector = new Vector3();
            vector->x = radius * sinf( (c) * M_PI_Divided_By_180 ) * sinf( (z + degreeIncrement) * M_PI_Divided_By_180 );
            vector->y = radius * cosf( (c) * M_PI_Divided_By_180 ) * sinf( (z + degreeIncrement) * M_PI_Divided_By_180 );
            vector->z = radius * cosf( (z + degreeIncrement) * M_PI_Divided_By_180 );
            vectors->push_back(vector);

            vector = new Vector3();
            vector->x = radius * sinf( (c + degreeIncrement) * M_PI_Divided_By_180 ) * sinf( (z) * M_PI_Divided_By_180 );
            vector->y = radius * cosf( (c + degreeIncrement) * M_PI_Divided_By_180 ) * sinf( (z) * M_PI_Divided_By_180 );
            vector->z = radius * cosf( (z) * M_PI_Divided_By_180 );
            vectors->push_back(vector);
            
            vector = new Vector3();
            vector->x = radius * sinf( (c + degreeIncrement) * M_PI_Divided_By_180 ) * sinf( (z + degreeIncrement) * M_PI_Divided_By_180 );
            vector->y = radius * cosf( (c + degreeIncrement) * M_PI_Divided_By_180 ) * sinf( (z + degreeIncrement) * M_PI_Divided_By_180 );
            vector->z = radius * cosf( (z + degreeIncrement) * M_PI_Divided_By_180 );
            vectors->push_back(vector);
        }
    }
    
    return vectors;
}

vector<Vector3 *> * MainEngine::createPlan()
{
    real x = 3.0f;
    real y = 0.0f;
    real z = 3.0f;
    real inc = 0.0f;
    int lines = 31;

    vector<Vector3 *> * vectors = new vector<Vector3 *>();
    
    for (int i=0; i<lines; i++) {
        vectors->push_back(new Vector3(x - inc, y, z));
        vectors->push_back(new Vector3(x - inc, y, -z));
        
        inc+=0.2;
    }
    
    inc = 0;
    for (int i=0; i<lines; i++) {
        vectors->push_back(new Vector3(x, y, z - inc));
        vectors->push_back(new Vector3(-x, y, z - inc));
        
        inc+=0.2;
    }
    
    return vectors;
}

vector<Vector3 *> * MainEngine::createBox()
{
    vector<Vector3 *> * vectors = new vector<Vector3 *>();
    vectors->push_back(new Vector3(0.1f, -0.1f, -0.1f));
    vectors->push_back(new Vector3(0.1f, 0.1f, -0.1f));
    vectors->push_back(new Vector3(0.1f, -0.1f, 0.1f));
    vectors->push_back(new Vector3(0.1f, -0.1f, 0.1f));
    vectors->push_back(new Vector3(0.1f, 0.1f, -0.1f));
    vectors->push_back(new Vector3(0.1f, 0.1f, 0.1f));
    
    vectors->push_back(new Vector3(0.1f, 0.1f, -0.1f));
    vectors->push_back(new Vector3(-0.1f, 0.1f, -0.1f));
    vectors->push_back(new Vector3(0.1f, 0.1f, 0.1f));
    vectors->push_back(new Vector3(0.1f, 0.1f, 0.1f));
    vectors->push_back(new Vector3(-0.1f, 0.1f, -0.1f));
    vectors->push_back(new Vector3(-0.1f, 0.1f, 0.1f));
    
    vectors->push_back(new Vector3(-0.1f, 0.1f, -0.1f));
    vectors->push_back(new Vector3(-0.1f, -0.1f, -0.1f));
    vectors->push_back(new Vector3(-0.1f, 0.1f, 0.1f));
    vectors->push_back(new Vector3(-0.1f, 0.1f, 0.1f));
    vectors->push_back(new Vector3(-0.1f, -0.1f, -0.1f));
    vectors->push_back(new Vector3(-0.1f, -0.1f, 0.1f));
    
    vectors->push_back(new Vector3(-0.1f, -0.1f, -0.1f));
    vectors->push_back(new Vector3(0.1f, -0.1f, -0.1f));
    vectors->push_back(new Vector3(-0.1f, -0.1f, 0.1f));
    vectors->push_back(new Vector3(-0.1f, -0.1f, 0.1f));
    vectors->push_back(new Vector3(0.1f, -0.1f, -0.1f));
    vectors->push_back(new Vector3(0.1f, -0.1f, 0.1f));
    
    vectors->push_back(new Vector3(0.1f, 0.1f, 0.1f));
    vectors->push_back(new Vector3(-0.1f, 0.1f, 0.1f));
    vectors->push_back(new Vector3(0.1f, -0.1f, 0.1f));
    vectors->push_back(new Vector3(0.1f, -0.1f, 0.1f));
    vectors->push_back(new Vector3(-0.1f, 0.1f, 0.1f));
    vectors->push_back(new Vector3(-0.1f, -0.1f, 0.1f));
    
    vectors->push_back(new Vector3(0.1f, -0.1f, -0.1f));
    vectors->push_back(new Vector3(-0.1f, -0.1f, -0.1f));
    vectors->push_back(new Vector3(0.1f, 0.1f, -0.1f));
    vectors->push_back(new Vector3(0.1f, 0.1f, -0.1f));
    vectors->push_back(new Vector3(-0.1f, -0.1f, -0.1f));
    vectors->push_back(new Vector3(-0.1f, 0.1f, -0.1f));
    
    return vectors;
}

vector<Vector3 *> * MainEngine::createTriangleWithTriangularBase()
{
    vector<Vector3 *> * vectors = new vector<Vector3 *>();
    vectors->push_back(new Vector3(0.1f, 0.0f, 0.0f));
    vectors->push_back(new Vector3(-0.1f, 0.0f, 0.0f));
    vectors->push_back(new Vector3(0.0f, 0.0f, 0.1f));
    vectors->push_back(new Vector3(0.1f, 0.0f, 0.0f));
    
    vectors->push_back(new Vector3(0.1f, 0.0f, 0.0f));
    vectors->push_back(new Vector3(-0.1f, 0.0f, 0.0f));
    vectors->push_back(new Vector3(0.0f, 0.1f, 0.0f));
    vectors->push_back(new Vector3(0.1f, 0.0f, 0.0f));
    
    vectors->push_back(new Vector3(0.1f, 0.0f, 0.0f));
    vectors->push_back(new Vector3(0.0f, 0.0f, 0.1f));
    vectors->push_back(new Vector3(0.0f, 0.1f, 0.0f));
    vectors->push_back(new Vector3(0.1f, 0.0f, 0.0f));
    
    vectors->push_back(new Vector3(0.0f, 0.0f, 0.1f));
    vectors->push_back(new Vector3(-0.1f, 0.0f, 0.0f));
    vectors->push_back(new Vector3(0.0f, 0.1f, 0.0f));
    vectors->push_back(new Vector3(0.0f, 0.0f, 0.1f));

    return vectors;
}

vector<Vector3 *> * MainEngine::createTriangleWithSquareBase()
{
    vector<Vector3 *> * vectors = new vector<Vector3 *>();
    vectors->push_back(new Vector3(0.05f, 0.0f, -0.05f));
    vectors->push_back(new Vector3(0.05f, 0.0f, 0.05f));
    vectors->push_back(new Vector3(-0.05f, 0.0f, 0.05f));
    vectors->push_back(new Vector3(-0.05f, 0.0f, -0.05f));
    vectors->push_back(new Vector3(0.05f, 0.0f, -0.05f));
    
    vectors->push_back(new Vector3(0.05f, 0.0f, -0.05f));
    vectors->push_back(new Vector3(0.05f, 0.0f, 0.05f));
    vectors->push_back(new Vector3(0.0f, 0.1f, 0.0f));
    vectors->push_back(new Vector3(0.05f, 0.0f, -0.05f));
    
    vectors->push_back(new Vector3(0.05f, 0.0f, 0.05f));
    vectors->push_back(new Vector3(-0.05f, 0.0f, 0.05f));
    vectors->push_back(new Vector3(0.0f, 0.1f, 0.0f));
    vectors->push_back(new Vector3(0.05f, 0.0f, 0.05f));
    
    vectors->push_back(new Vector3(-0.05f, 0.0f, 0.05f));
    vectors->push_back(new Vector3(-0.05f, 0.0f, -0.05f));
    vectors->push_back(new Vector3(0.0f, 0.1f, 0.0f));
    vectors->push_back(new Vector3(-0.05f, 0.0f, 0.05f));
    
    vectors->push_back(new Vector3(-0.05f, 0.0f, -0.05f));
    vectors->push_back(new Vector3(0.05f, 0.0f, -0.05f));
    vectors->push_back(new Vector3(0.0f, 0.1f, 0.0f));
    vectors->push_back(new Vector3(-0.05f, 0.0f, -0.05f));
                                    
    return vectors;
}

/*
CONE
 
 v -0.329893 -0.244687 -0.965621
 v -0.134803 -0.244687 -0.946406
 v 0.052790 -0.244687 -0.889500
 v 0.225677 -0.244687 -0.797090
 v 0.377213 -0.244687 -0.672727
 v 0.501576 -0.244687 -0.521191
 v 0.593986 -0.244687 -0.348304
 v 0.650892 -0.244687 -0.160711
 v 0.670107 -0.244687 0.034379
 v -0.329893 1.755313 0.034379
 v 0.650892 -0.244687 0.229470
 v 0.593986 -0.244687 0.417063
 v 0.501576 -0.244687 0.589949
 v 0.377213 -0.244687 0.741486
 v 0.225677 -0.244687 0.865849
 v 0.052790 -0.244687 0.958259
 v -0.134803 -0.244687 1.015165
 v -0.329894 -0.244687 1.034379
 v -0.524984 -0.244687 1.015164
 v -0.712577 -0.244687 0.958259
 v -0.885464 -0.244687 0.865849
 v -1.037001 -0.244687 0.741486
 v -1.161363 -0.244687 0.589949
 v -1.253773 -0.244687 0.417062
 v -1.310679 -0.244687 0.229469
 v -1.329893 -0.244687 0.034378
 v -1.310679 -0.244687 -0.160712
 v -1.253773 -0.244687 -0.348305
 v -1.161362 -0.244687 -0.521192
 v -1.036999 -0.244687 -0.672728
 v -0.885463 -0.244687 -0.797091
 v -0.712576 -0.244687 -0.889501
 v -0.524982 -0.244687 -0.946406
*/