//
//  ObjectsViewController.m
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 14/07/12.
//  
//

#import "ObjectsViewController.h"

@implementation ObjectsViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
}

- (void)startSimulation:(NSTimer*)theTimer
{
    Controller::getInstance()->startSimulation();
}

- (void)viewDidUnload
{
    [super viewDidUnload];
    // Release any retained subviews of the main view.
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
	return UIInterfaceOrientationIsLandscape(interfaceOrientation);
}

- (IBAction)actionStartSimulation:(id)sender 
{
    [[UIApplication sharedApplication] setStatusBarHidden:YES withAnimation:UIStatusBarAnimationNone];
    Controller::getInstance()->setEditMode(false);

    // start simulation after 2 seconds
    [NSTimer scheduledTimerWithTimeInterval:2
                                     target:self
                                   selector:@selector(startSimulation:)
                                   userInfo:NULL
                                    repeats:NO];
}

- (IBAction)actionEditSimulation:(id)sender
{
    [[UIApplication sharedApplication] setStatusBarHidden:YES withAnimation:UIStatusBarAnimationNone];
    Controller::getInstance()->editSimulation();
}

- (IBAction)actionCircle:(id)sender
{
//#if defined (_3D_)
//    Controller::getInstance()->createSimulatedObject3D(SPHERE);
//#else
//    Controller::getInstance()->createSimulatedObject2D(CIRCLE);
//#endif
    Controller::getInstance()->setTypeNextObject(SPHERE);
}

- (IBAction)actionSquare:(id)sender
{
//#if defined (_3D_)
//    Controller::getInstance()->createSimulatedObject3D(BOX);
//#else
//    Controller::getInstance()->createSimulatedObject2D(SQUARE);
//#endif
    Controller::getInstance()->setTypeNextObject(BOX);
}

- (IBAction)actionTriangle:(id)sender
{
//#if defined (_3D_)
//    Controller::getInstance()->createSimulatedObject3D(TRIANGLE_SQUARE_BASE);
//#else
//    Controller::getInstance()->createSimulatedObject2D(TRIANGLE);
//#endif
    Controller::getInstance()->setTypeNextObject(PYRAMID);
}

- (IBAction)actionSurfaceScene:(id)sender
{
//#if defined (_3D_)
//    Controller::getInstance()->createSimulatedObject3D(PLAN);
//#else
//    Controller::getInstance()->createSimulatedObject2D(PLAN);
//#endif
    Controller::getInstance()->setTypeNextObject(PLAN);
}

- (IBAction)actionClearSimulation:(id)sender
{
    Controller::getInstance()->clearSimularion();
}

- (IBAction)actionSaveSimulation:(id)sender
{
    self.tabBarController.selectedIndex = 2;
    
    SaveSimulationViewController * saveController = (SaveSimulationViewController *) self.tabBarController.selectedViewController;
    std::string strSimulation = Controller::getInstance()->generateSimulationToCharacter();

    if (strSimulation.size() <= 0) {
        return;
    }

    NSString * nsSimulation = [NSString stringWithCString:strSimulation.c_str() encoding:NSASCIIStringEncoding];
    saveController.areaText.text = nsSimulation;
}

@end
