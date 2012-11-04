//
//  ObjectsViewController.h
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 14/07/12.
//  
//

#import <UIKit/UIKit.h>

#include "SaveSimulationViewController.h"
#include "ConfigurationViewController.h"
#include "Controller.h"

@interface ObjectsViewController : UIViewController

- (IBAction)actionStartSimulation:(id)sender;

- (IBAction)actionEditSimulation:(id)sender;

- (IBAction)actionCircle:(id)sender;

- (IBAction)actionSquare:(id)sender;

- (IBAction)actionTriangle:(id)sender;

- (IBAction)actionClearSimulation:(id)sender;

- (IBAction)actionSurfaceScene:(id)sender;

- (IBAction)actionSaveSimulation:(id)sender;

- (IBAction)actionGenerateRandomSimulation:(id)sender;
@end
