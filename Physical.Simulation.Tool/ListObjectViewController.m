//
//  ListObjectViewController.m
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 13/10/12.
//
//

#import "ListObjectViewController.h"

@interface ListObjectViewController ()
@property(nonatomic) NSArray * listData;
@end

@implementation ListObjectViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view.

    [self loadObjects];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)loadObjects
{
    std::vector<SimulatedObject *> * objects = Controller::getInstance()->getSimulatedObjects();

    NSMutableArray * arrayObjects = [[NSMutableArray alloc]init];
    SimulatedObject * object = NULL;
    NSValue * value = NULL;

    for (int i=0; i< objects->size(); i++) {
        object = objects->at(i);
        value = [NSValue valueWithPointer:object];
        [arrayObjects addObject:value];
    }

    self.listData = arrayObjects;

    [self.tableViewObjects reloadData];
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    NSValue * value = [self.listData objectAtIndex:indexPath.item];
    SimulatedObject * object = (SimulatedObject*)[value pointerValue];
//    NSString * name = [[NSString alloc]initWithCString: object->getId() encoding:NSASCIIStringEncoding];
//    ConfigurationViewController * configuration = [[ConfigurationViewController alloc]init];
    ConfigurationViewController * configuration = [self.storyboard instantiateViewControllerWithIdentifier:@"idConfigurationViewController"];
    [configuration setSimulatedObject:object];
    [self.navigationController pushViewController:configuration animated:TRUE];
}

- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath;
{
    if (editingStyle == UITableViewCellEditingStyleDelete) {
        NSValue * value = [self.listData objectAtIndex: [indexPath item]];
        SimulatedObject * object = (SimulatedObject*)[value pointerValue];

        Controller::getInstance()->deleteSimulatedObject(object);

        [self loadObjects];
	}
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return [self.listData count];
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *SimpleTableIdentifier = @"SimpleTableIdentifier";
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:SimpleTableIdentifier];
    if (cell == nil) {
        cell = [[UITableViewCell alloc]initWithStyle:UITableViewCellStyleDefault reuseIdentifier:SimpleTableIdentifier];
    }

    NSValue * value = [self.listData objectAtIndex: [indexPath row]];
    SimulatedObject * object = (SimulatedObject*)[value pointerValue];

    cell.textLabel.text = [[NSString alloc]initWithCString: object->getId() encoding:NSASCIIStringEncoding];

    switch (object->getTypeObject()) {
        case PLAN:
            cell.imageView.image = [UIImage imageNamed:@"plane.png"];
            break;

        case BOX:
            cell.imageView.image = [UIImage imageNamed:@"square.png"];
            break;

        case SPHERE:
            cell.imageView.image = [UIImage imageNamed:@"circle.png"];
            break;

        default:
            break;
    }

    //cell.imageView.image = [UIImage imageNamed:@"albert.icon.72x72.png"];
    cell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
    cell.selectionStyle = UITableViewCellSelectionStyleBlue;

//	UIImage *image = [UIImage imageNamed:@"button_play.png"];
//
//	UIButton *button = [UIButton buttonWithType:UIButtonTypeCustom];
//	CGRect frame = CGRectMake(0.0, 0.0, 50, 50);
//	button.frame = frame;
//
//	[button setBackgroundImage:image forState:UIControlStateNormal];

	// set the button's target to this table view controller so we can interpret touch events and map that to a NSIndexSet
//	[button addTarget:self action:@selector(checkButtonTapped:event:) forControlEvents:UIControlEventTouchUpInside];
//	cell.backgroundColor = [UIColor clearColor];
//	cell.accessoryView = button;

    return cell;
}

- (IBAction)actionVoltar:(id)sender
{
//    SimulationViewController * simulation = [self.storyboard instantiateViewControllerWithIdentifier:@"idSimulationViewController"];

}
@end
