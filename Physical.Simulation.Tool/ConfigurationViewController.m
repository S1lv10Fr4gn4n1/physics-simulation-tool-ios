//
//  ConfigurationViewController.m
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 29/09/12.
//
//

#import "ConfigurationViewController.h"

@interface ConfigurationViewController ()
-(void)resetValues;
-(void)resetValuesLabels;
-(void)resetValuesComponents;
@end

@implementation ConfigurationViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view.

    if (Controller::getInstance()->getTypeNextObject() == PLAN) {
        self.viewMoreConfig.hidden = TRUE;
    }

    [self resetValues];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (IBAction)actionSaveAdd:(id)sender
{
    if (Controller::getInstance()->getTypeNextObject() == PLAN &&
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


    SimulatedObject * object = Controller::getInstance()->makeSimulatedObject3D();
    object->setFriction(self.sliderFriction.value);
    object->setRestitution(self.sliderRestitution.value);
    object->setLinearDamping(self.sliderLinearDamping.value);
    object->setAngularDamping(self.sliderAngularDamping.value);
    object->setMass(self.sliderMass.value);
    object->setCanSleep(self.switchAwakeup.on);
    object->setPosition(self.sliderPositionX.value, self.sliderPositionY.value, self.sliderPositionZ.value);
    object->setVelocity(self.sliderVelocityX.value, self.sliderVelocityY.value, self.sliderVelocityZ.value);
    object->setRotation(self.sliderRotationX.value, self.sliderRotationY.value, self.sliderRotationZ.value);
    object->setAcceleration(self.sliderAccelerationX.value, self.sliderAccelerationY.value, self.sliderAccelerationZ.value);
    object->addForce(Vector3(self.sliderForceX.value, self.sliderForceY.value, self.sliderForceZ.value));
    Vector3 gravity(0.0f, -self.sliderAccelerationGravity.value, 0.0f);

    if (Controller::getInstance()->getTypeNextObject() == PLAN) {
        object->setPosition(0.0f, 0.0f, 0.0f);
        object->setMass(0.0f);
    }

    Controller::getInstance()->addAndInitSimulatedObject3D(object, gravity);
    [self dismissViewControllerAnimated:TRUE completion:NULL];
}

- (IBAction)actionDefaultValues:(id)sender
{
    [self resetValues];
}

- (IBAction)actionGoBack:(id)sender
{
    [self dismissViewControllerAnimated:TRUE completion:NULL];
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
    [self resetValuesLabels];
    [self resetValuesComponents];
}

-(void)resetValuesLabels
{
    self.labelAccelerationGravity.text = [NSString stringWithFormat:@"%2.5f", ACCELERATION_GRAVITY];
    self.labelFriction.text = [NSString stringWithFormat:@"%2.5f", COEF_FRICTION];
    self.labelRestitution.text = [NSString stringWithFormat:@"%2.5f", COEF_RESTITUTION];
    self.labelLinearDamping.text = [NSString stringWithFormat:@"%2.5f", COEF_LINEAR_DAMPING];
    self.labelAngularDamping.text = [NSString stringWithFormat:@"%2.5f", COEF_ANGULAR_DAMPING];
    self.labelMass.text = [NSString stringWithFormat:@"%2.5f", MASS];
    
    self.labelPositionX.text = [NSString stringWithFormat:@"%2.5f", POSITION_X];
    self.labelPositionY.text = [NSString stringWithFormat:@"%2.5f", POSITION_Y];
    self.labelPositionZ.text = [NSString stringWithFormat:@"%2.5f", POSITION_Z];

    self.labelVelocityX.text = [NSString stringWithFormat:@"%2.5f", 0.0f];
    self.labelVelocityY.text = [NSString stringWithFormat:@"%2.5f", 0.0f];
    self.labelVelocityZ.text = [NSString stringWithFormat:@"%2.5f", 0.0f];

    self.labelRotationX.text = [NSString stringWithFormat:@"%2.5f", 0.0f];
    self.labelRotationY.text = [NSString stringWithFormat:@"%2.5f", 0.0f];
    self.labelRotationZ.text = [NSString stringWithFormat:@"%2.5f", 0.0f];

    self.labelAccelerationX.text = [NSString stringWithFormat:@"%2.5f", 0.0f];
    self.labelAccelerationY.text = [NSString stringWithFormat:@"%2.5f", 0.0f];
    self.labelAccelerationZ.text = [NSString stringWithFormat:@"%2.5f", 0.0f];

    self.labelForceX.text = [NSString stringWithFormat:@"%2.5f", 0.0f];
    self.labelForceY.text = [NSString stringWithFormat:@"%2.5f", 0.0f];
    self.labelForceZ.text = [NSString stringWithFormat:@"%2.5f", 0.0f];
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

@end
