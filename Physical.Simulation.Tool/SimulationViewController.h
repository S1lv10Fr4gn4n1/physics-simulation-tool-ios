//
//  ViewController.h
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani da Silva on 11/06/12.
//  
//

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>

#include "Controller.h"

//!Class controller design simulation.
//!Containing the openGL context and callback interface for interaction with the simulation.
@interface SimulationViewController : GLKViewController

@property (weak, nonatomic) IBOutlet UILabel *labelHelp;
@property (weak, nonatomic) IBOutlet UIButton *buttonFind;
- (IBAction)actionFind:(id)sender;

@end
