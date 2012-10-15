//
//  ConfigurationViewController.m
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 29/09/12.
//
//

#import "ConfigurationViewController.h"

@interface ConfigurationViewController ()
{
    bool inclusion;
}
-(void)resetValues;
-(void)resetValuesLabels;
-(void)resetValuesComponents;

@property(nonatomic) SimulatedObject * object;

@end

@implementation ConfigurationViewController

- (void)viewDidLoad
{
    inclusion = true;

    [super viewDidLoad];
	// Do any additional setup after loading the view.

    if (self.object == NULL && Controller::getInstance()->getTypeNextObject() == PLAN) {
        self.viewMoreConfig.hidden = TRUE;
    }

    if (self.object != NULL && self.object->getTypeObject() == PLAN) {
        self.viewMoreConfig.hidden = TRUE;
    }

    if (self.object != NULL) {
        inclusion = false;

        [self loadValuesFromObject];
        [self resetValuesLabels];

        CGSize containerSize = CGSizeMake(1024.0f, 748.0f);
        self.scrollview.contentSize = containerSize;
    } else {
        [self resetValues];
    }
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)setSimulatedObject:(SimulatedObject *) _simulatedObject
{
    self.object = _simulatedObject;
}

- (IBAction)actionSaveAdd:(id)sender
{
    if (inclusion &&
        Controller::getInstance()->getTypeNextObject() == PLAN &&
        Controller::getInstance()->alreadyExistPlan()) {
        UIAlertView * alert = [[UIAlertView alloc]initWithTitle:@"Atenção"
                                                        message:@"Já existe um Plano na cena!"
                                                       delegate:self
                                              cancelButtonTitle:@"OK"
                                              otherButtonTitles: nil];
        [alert show];

        [self dismissViewControllerAnimated:TRUE completion:NULL];
        
        return;
    }

    if (inclusion) {
        self.object = Controller::getInstance()->makeSimulatedObject3D();
    }
    self.object->setFriction(self.sliderFriction.value);
    self.object->setRestitution(self.sliderRestitution.value);
    self.object->setLinearDamping(self.sliderLinearDamping.value);
    self.object->setAngularDamping(self.sliderAngularDamping.value);
    self.object->setMass(self.sliderMass.value);
    self.object->setCanSleep(self.switchAwakeup.on);
    self.object->setPosition(self.sliderPositionX.value, self.sliderPositionY.value, self.sliderPositionZ.value);
    self.object->setVelocity(self.sliderVelocityX.value, self.sliderVelocityY.value, self.sliderVelocityZ.value);
    self.object->setRotation(self.sliderRotationX.value, self.sliderRotationY.value, self.sliderRotationZ.value);
    self.object->setAcceleration(self.sliderAccelerationX.value, self.sliderAccelerationY.value, self.sliderAccelerationZ.value);
    self.object->addForce(Vector3(self.sliderForceX.value, self.sliderForceY.value, self.sliderForceZ.value));
    Vector3 gravity(0.0f, -self.sliderAccelerationGravity.value, 0.0f);

    if (inclusion && Controller::getInstance()->getTypeNextObject() == PLAN) {
        self.object->setPosition(0.0f, 0.0f, 0.0f);
        self.object->setMass(0.0f);
    }

    if (inclusion) {
        Controller::getInstance()->addAndInitSimulatedObject3D(self.object, gravity);
        [self dismissViewControllerAnimated:TRUE completion:NULL];
    } else {
        self.object->updateMatrixTransformation();
        [self.navigationController popToRootViewControllerAnimated:TRUE];
    }

}

- (IBAction)actionDefaultValues:(id)sender
{
    [self resetValues];
}

- (IBAction)actionGoBack:(id)sender
{
    if (inclusion) {
        [self dismissViewControllerAnimated:TRUE completion:NULL];
    } else {
        [self.navigationController popToRootViewControllerAnimated:TRUE];
    }
}

- (IBAction)valueChangeAccelerationGravity:(id)sender
{
    self.labelAccelerationGravity.text = [NSString stringWithFormat:@"%2.5f", self.sliderAccelerationGravity.value];
}

- (IBAction)valueChangeFriction:(id)sender
{
    self.labelFriction.text = [NSString stringWithFormat:@"%2.5f", self.sliderFriction.value];
}

- (IBAction)valueChangeRestitution:(id)sender
{
    self.labelRestitution.text = [NSString stringWithFormat:@"%2.5f", self.sliderRestitution.value];
}

- (IBAction)valueChangeaLinearDamping:(id)sender
{
    self.labelLinearDamping.text = [NSString stringWithFormat:@"%2.5f", self.sliderLinearDamping.value];
}

- (IBAction)valueChangeAngularDamping:(id)sender
{
    self.labelAngularDamping.text = [NSString stringWithFormat:@"%2.5f", self.sliderAngularDamping.value];
}

- (IBAction)valueChangeMass:(id)sender
{
    self.labelMass.text = [NSString stringWithFormat:@"%2.5f", self.sliderMass.value];
}

- (IBAction)valueChangeAwakeup:(id)sender
{
}

- (IBAction)valueChangePositionX:(id)sender
{
    self.labelPositionX.text = [NSString stringWithFormat:@"%2.5f", self.sliderPositionX.value];
}

- (IBAction)valueChangePositionY:(id)sender
{
    self.labelPositionY.text = [NSString stringWithFormat:@"%2.5f", self.sliderPositionY.value];
}

- (IBAction)valueChangePositionZ:(id)sender
{
    self.labelPositionZ.text = [NSString stringWithFormat:@"%2.5f", self.sliderPositionZ.value];
}

- (IBAction)valueChangeVelocityX:(id)sender
{
    self.labelVelocityX.text = [NSString stringWithFormat:@"%2.5f", self.sliderVelocityX.value];
}

- (IBAction)valueChangeVelocityY:(id)sender
{
    self.labelVelocityY.text = [NSString stringWithFormat:@"%2.5f", self.sliderVelocityY.value];
}

- (IBAction)valueChangeVelocityZ:(id)sender
{
    self.labelVelocityZ.text = [NSString stringWithFormat:@"%2.5f", self.sliderVelocityZ.value];
}

- (IBAction)valueChangeRotationX:(id)sender
{
    self.labelRotationX.text = [NSString stringWithFormat:@"%2.5f", self.sliderRotationX.value];
}

- (IBAction)valueChangeRotationY:(id)sender
{
    self.labelRotationY.text = [NSString stringWithFormat:@"%2.5f", self.sliderRotationY.value];
}

- (IBAction)valueChangeRotationZ:(id)sender
{
    self.labelRotationZ.text = [NSString stringWithFormat:@"%2.5f", self.sliderRotationZ.value];
}

- (IBAction)valueChangeAccelerationX:(id)sender
{
    self.labelAccelerationX.text = [NSString stringWithFormat:@"%2.5f", self.sliderAccelerationX.value];
}

- (IBAction)valueChangeAccelerationY:(id)sender
{
    self.labelAccelerationY.text = [NSString stringWithFormat:@"%2.5f", self.sliderAccelerationY.value];
}

- (IBAction)valueChangeAccelerationZ:(id)sender
{
    self.labelAccelerationZ.text = [NSString stringWithFormat:@"%2.5f", self.sliderAccelerationZ.value];
}

- (IBAction)valueChangeForceX:(id)sender
{
    self.labelForceX.text = [NSString stringWithFormat:@"%2.5f", self.sliderForceX.value];
}

- (IBAction)valueChangeForceY:(id)sender
{
    self.labelForceY.text = [NSString stringWithFormat:@"%2.5f", self.sliderForceY.value];
}

- (IBAction)valueChangeForceZ:(id)sender
{
    self.labelForceZ.text = [NSString stringWithFormat:@"%2.5f", self.sliderForceZ.value];
}

-(void)resetValues
{
    [self resetValuesComponents];
    [self resetValuesLabels];
}

-(void)resetValuesLabels
{
    self.labelAccelerationGravity.text = [NSString stringWithFormat:@"%2.5f", self.sliderAccelerationGravity.value];
    self.labelFriction.text = [NSString stringWithFormat:@"%2.5f", self.sliderFriction.value];
    self.labelRestitution.text = [NSString stringWithFormat:@"%2.5f", self.sliderRestitution.value];
    self.labelLinearDamping.text = [NSString stringWithFormat:@"%2.5f", self.sliderLinearDamping.value];
    self.labelAngularDamping.text = [NSString stringWithFormat:@"%2.5f", self.sliderAngularDamping.value];
    self.labelMass.text = [NSString stringWithFormat:@"%2.5f", self.sliderMass.value];
    
    self.labelPositionX.text = [NSString stringWithFormat:@"%2.5f", self.sliderPositionX.value];
    self.labelPositionY.text = [NSString stringWithFormat:@"%2.5f", self.sliderPositionY.value];
    self.labelPositionZ.text = [NSString stringWithFormat:@"%2.5f", self.sliderPositionZ.value];

    self.labelVelocityX.text = [NSString stringWithFormat:@"%2.5f", self.sliderVelocityX.value];
    self.labelVelocityY.text = [NSString stringWithFormat:@"%2.5f", self.sliderVelocityY.value];
    self.labelVelocityZ.text = [NSString stringWithFormat:@"%2.5f", self.sliderVelocityZ.value];

    self.labelRotationX.text = [NSString stringWithFormat:@"%2.5f", self.sliderRotationX.value];
    self.labelRotationY.text = [NSString stringWithFormat:@"%2.5f", self.sliderRotationY.value];
    self.labelRotationZ.text = [NSString stringWithFormat:@"%2.5f", self.sliderRotationZ.value];

    self.labelAccelerationX.text = [NSString stringWithFormat:@"%2.5f", self.sliderAccelerationX.value];
    self.labelAccelerationY.text = [NSString stringWithFormat:@"%2.5f", self.sliderAccelerationY.value];
    self.labelAccelerationZ.text = [NSString stringWithFormat:@"%2.5f", self.sliderAccelerationZ.value];

    self.labelForceX.text = [NSString stringWithFormat:@"%2.5f", self.sliderForceX.value];
    self.labelForceY.text = [NSString stringWithFormat:@"%2.5f", self.sliderForceY.value];
    self.labelForceZ.text = [NSString stringWithFormat:@"%2.5f", self.sliderForceZ.value];
}

-(void)resetValuesComponents
{
    self.sliderAccelerationGravity.value = ACCELERATION_GRAVITY;
    self.sliderFriction.value = COEF_FRICTION;
    self.sliderRestitution.value = COEF_RESTITUTION;
    self.sliderLinearDamping.value = COEF_LINEAR_DAMPING;
    self.sliderAngularDamping.value = COEF_ANGULAR_DAMPING;
    self.sliderMass.value = MASS;

    self.switchAwakeup.on = CAN_AWAKE;

    self.sliderPositionX.value = POSITION_X;
    self.sliderPositionY.value = POSITION_Y;
    self.sliderPositionZ.value = POSITION_Z;

    self.sliderVelocityX.value = 0.0f;
    self.sliderVelocityY.value = 0.0f;
    self.sliderVelocityZ.value = 0.0f;

    self.sliderRotationX.value = 0.0f;
    self.sliderRotationY.value = 0.0f;
    self.sliderRotationZ.value = 0.0f;

    self.sliderAccelerationX.value = 0.0f;
    self.sliderAccelerationY.value = 0.0f;
    self.sliderAccelerationZ.value = 0.0f;

    self.sliderForceX.value = 0.0f;
    self.sliderForceY.value = 0.0f;
    self.sliderForceZ.value = 0.0f;
}

-(void)loadValuesFromObject
{
    self.sliderAccelerationGravity.value = real_abs(self.object->getAccelerationGravity().y);
    self.sliderFriction.value = self.object->getFriction();
    self.sliderRestitution.value = self.object->getRestitution();
    self.sliderLinearDamping.value = self.object->getLinearDamping();
    self.sliderAngularDamping.value = self.object->getAngularDamping();
    self.sliderMass.value = self.object->getMass();

    self.switchAwakeup.on = self.object->isCanSleep();

    self.sliderPositionX.value = self.object->getPosition().x;
    self.sliderPositionY.value = self.object->getPosition().y;
    self.sliderPositionZ.value = self.object->getPosition().z;

    self.sliderVelocityX.value = self.object->getVelocity().x;
    self.sliderVelocityY.value = self.object->getVelocity().y;
    self.sliderVelocityZ.value = self.object->getVelocity().z;

    self.sliderRotationX.value = self.object->getRotation().x;
    self.sliderRotationY.value = self.object->getRotation().y;
    self.sliderRotationZ.value = self.object->getRotation().z;

    self.sliderAccelerationX.value = self.object->getAcceleration().x;
    self.sliderAccelerationY.value = self.object->getAcceleration().y;
    self.sliderAccelerationZ.value = self.object->getAcceleration().z;

    self.sliderForceX.value = self.object->getForceAccum().x;
    self.sliderForceY.value = self.object->getForceAccum().y;
    self.sliderForceZ.value = self.object->getForceAccum().z;
}

@end
