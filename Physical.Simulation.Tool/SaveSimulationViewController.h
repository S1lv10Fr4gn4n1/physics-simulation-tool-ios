//
//  SaveSimulationViewController.h
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 28/09/12.
//

#import <UIKit/UIKit.h>
#import "Controller.h"

@interface SaveSimulationViewController : UIViewController <UITableViewDelegate, UITableViewDataSource>

- (IBAction)actionSave:(id)sender;
- (IBAction)actionClean:(id)sender;

@property (weak, nonatomic) IBOutlet UITextField *imputFileName;
@property (weak, nonatomic) IBOutlet UITextView *areaText;
@property (weak, nonatomic) IBOutlet UITableView *tableView;

@end
