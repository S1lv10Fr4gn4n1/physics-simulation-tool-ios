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
#if defined (_3D_)
    Controller::getInstance()->createSimulatedObject3D(SPHERE);
#else
    Controller::getInstance()->createSimulatedObject2D(CIRCLE);
#endif
}

- (IBAction)actionSquare:(id)sender
{
#if defined (_3D_)
    Controller::getInstance()->createSimulatedObject3D(BOX);
#else
    Controller::getInstance()->createSimulatedObject2D(SQUARE);
#endif
}

- (IBAction)actionTriangle:(id)sender
{
#if defined (_3D_)
    Controller::getInstance()->createSimulatedObject3D(TRIANGLE_SQUARE_BASE);
#else
    Controller::getInstance()->createSimulatedObject2D(TRIANGLE);
#endif
}

- (IBAction)actionClearSimulation:(id)sender
{
    Controller::getInstance()->clearSimularion();
}

- (IBAction)actionSurfaceScene:(id)sender
{
#if defined (_3D_)
    Controller::getInstance()->createSimulatedObject3D(PLAN);
#else
    Controller::getInstance()->createSimulatedObject2D(NONE); // TODO revise: PLAN
#endif
}

@end
