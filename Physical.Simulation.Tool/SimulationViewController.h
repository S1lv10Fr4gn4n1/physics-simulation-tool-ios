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

@interface SimulationViewController : GLKViewController

@property (weak, nonatomic) IBOutlet UIButton *buttonFind;
- (IBAction)actionFind:(id)sender;

@end
