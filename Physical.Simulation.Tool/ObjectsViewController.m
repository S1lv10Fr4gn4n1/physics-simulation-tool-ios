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
    
//    // TODO for Tests
//    for (int i=0; i< 500; i++) {
//        Controller::getInstance()->createSimulatedObject(SQUARE);
//        Controller::getInstance()->createSimulatedObject(CIRCLE);
//        Controller::getInstance()->createSimulatedObject(TRIANGLE);
//    }
}

- (IBAction)actionEditSimulation:(id)sender
{
    [[UIApplication sharedApplication] setStatusBarHidden:YES withAnimation:UIStatusBarAnimationNone];
    Controller::getInstance()->editSimulation();
}

- (IBAction)actionCircle:(id)sender
{
    Controller::getInstance()->createSimulatedObject(CIRCLE);
}

- (IBAction)actionSquare:(id)sender
{
    Controller::getInstance()->createSimulatedObject(SQUARE);
}

- (IBAction)actionTriangle:(id)sender 
{
    Controller::getInstance()->createSimulatedObject(TRIANGLE);
}

- (IBAction)actionClearSimulation:(id)sender
{
    Controller::getInstance()->clearSimularion();
}

- (IBAction)actionSurfaceScene:(id)sender
{
    Controller::getInstance()->createSimulatedObject(PLAN);
}
@end
