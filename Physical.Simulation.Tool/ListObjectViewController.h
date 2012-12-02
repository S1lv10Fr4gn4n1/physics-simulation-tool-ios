//
//  ListObjectViewController.h
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 13/10/12.
//
//

#import <UIKit/UIKit.h>
#import "Controller.h"
#import "ConfigurationViewController.h"
#import "SimulationViewController.h"


//!Class controller from the list of objects available in the simulation
@interface ListObjectViewController : UIViewController<UITableViewDelegate, UITableViewDataSource>

@property (weak, nonatomic) IBOutlet UITableView *tableViewObjects;
- (IBAction)actionVoltar:(id)sender;

@end
