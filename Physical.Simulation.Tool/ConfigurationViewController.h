//
//  ConfigurationViewController.h
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 29/09/12.
//
//

#import <UIKit/UIKit.h>
#include "Controller.h"

@interface ConfigurationViewController : UIViewController

@property (strong, nonatomic) IBOutlet UIScrollView *scrollview;

- (void)setSimulatedObject:(SimulatedObject *) _simulatedObject;

- (IBAction)actionSaveAdd:(id)sender;
- (IBAction)actionDefaultValues:(id)sender;
- (IBAction)actionGoBack:(id)sender;

// acceleration gravity
- (IBAction)valueChangeAccelerationGravity:(id)sender;
@property (weak, nonatomic) IBOutlet UILabel *labelAccelerationGravity;
@property (weak, nonatomic) IBOutlet UISlider *sliderAccelerationGravity;

// friction
- (IBAction)valueChangeFriction:(id)sender;
@property (weak, nonatomic) IBOutlet UILabel *labelFriction;
@property (weak, nonatomic) IBOutlet UISlider *sliderFriction;

// restitution
- (IBAction)valueChangeRestitution:(id)sender;
@property (weak, nonatomic) IBOutlet UILabel *labelRestitution;
@property (weak, nonatomic) IBOutlet UISlider *sliderRestitution;

// linear damping
- (IBAction)valueChangeaLinearDamping:(id)sender;
@property (weak, nonatomic) IBOutlet UILabel *labelLinearDamping;
@property (weak, nonatomic) IBOutlet UISlider *sliderLinearDamping;

// angular damping
- (IBAction)valueChangeAngularDamping:(id)sender;
@property (weak, nonatomic) IBOutlet UILabel *labelAngularDamping;
@property (weak, nonatomic) IBOutlet UISlider *sliderAngularDamping;

// mass
- (IBAction)valueChangeMass:(id)sender;
@property (weak, nonatomic) IBOutlet UILabel *labelMass;
@property (weak, nonatomic) IBOutlet UISlider *sliderMass;

// awakeup
- (IBAction)valueChangeAwakeup:(id)sender;
@property (weak, nonatomic) IBOutlet UISwitch *switchAwakeup;

// position
- (IBAction)valueChangePositionX:(id)sender;
- (IBAction)valueChangePositionY:(id)sender;
- (IBAction)valueChangePositionZ:(id)sender;
@property (weak, nonatomic) IBOutlet UILabel *labelPositionX;
@property (weak, nonatomic) IBOutlet UILabel *labelPositionY;
@property (weak, nonatomic) IBOutlet UILabel *labelPositionZ;
@property (weak, nonatomic) IBOutlet UISlider *sliderPositionX;
@property (weak, nonatomic) IBOutlet UISlider *sliderPositionY;
@property (weak, nonatomic) IBOutlet UISlider *sliderPositionZ;

// velocity
- (IBAction)valueChangeVelocityX:(id)sender;
- (IBAction)valueChangeVelocityY:(id)sender;
- (IBAction)valueChangeVelocityZ:(id)sender;
@property (weak, nonatomic) IBOutlet UILabel *labelVelocityX;
@property (weak, nonatomic) IBOutlet UILabel *labelVelocityY;
@property (weak, nonatomic) IBOutlet UILabel *labelVelocityZ;
@property (weak, nonatomic) IBOutlet UISlider *sliderVelocityX;
@property (weak, nonatomic) IBOutlet UISlider *sliderVelocityY;
@property (weak, nonatomic) IBOutlet UISlider *sliderVelocityZ;

// rotation
- (IBAction)valueChangeRotationX:(id)sender;
- (IBAction)valueChangeRotationY:(id)sender;
- (IBAction)valueChangeRotationZ:(id)sender;
@property (weak, nonatomic) IBOutlet UILabel *labelRotationX;
@property (weak, nonatomic) IBOutlet UILabel *labelRotationY;
@property (weak, nonatomic) IBOutlet UILabel *labelRotationZ;
@property (weak, nonatomic) IBOutlet UISlider *sliderRotationX;
@property (weak, nonatomic) IBOutlet UISlider *sliderRotationY;
@property (weak, nonatomic) IBOutlet UISlider *sliderRotationZ;

// acceleration
- (IBAction)valueChangeAccelerationX:(id)sender;
- (IBAction)valueChangeAccelerationY:(id)sender;
- (IBAction)valueChangeAccelerationZ:(id)sender;
@property (weak, nonatomic) IBOutlet UILabel *labelAccelerationX;
@property (weak, nonatomic) IBOutlet UILabel *labelAccelerationY;
@property (weak, nonatomic) IBOutlet UILabel *labelAccelerationZ;
@property (weak, nonatomic) IBOutlet UISlider *sliderAccelerationX;
@property (weak, nonatomic) IBOutlet UISlider *sliderAccelerationY;
@property (weak, nonatomic) IBOutlet UISlider *sliderAccelerationZ;

// force
- (IBAction)valueChangeForceX:(id)sender;
- (IBAction)valueChangeForceY:(id)sender;
- (IBAction)valueChangeForceZ:(id)sender;
@property (weak, nonatomic) IBOutlet UILabel *labelForceX;
@property (weak, nonatomic) IBOutlet UILabel *labelForceY;
@property (weak, nonatomic) IBOutlet UILabel *labelForceZ;
@property (weak, nonatomic) IBOutlet UISlider *sliderForceX;
@property (weak, nonatomic) IBOutlet UISlider *sliderForceY;
@property (weak, nonatomic) IBOutlet UISlider *sliderForceZ;

@property (weak, nonatomic) IBOutlet UIView *viewMoreConfig;


@end
