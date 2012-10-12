//
//  SaveSimulationViewController.m
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 28/09/12.
//

#import "SaveSimulationViewController.h"

@interface SaveSimulationViewController ()
@property(nonatomic) NSArray * listData;
-(void)saveText:(NSString *)_text nameFile:(NSString *) _nameFile;
-(NSString *)displayContent:(NSString *)_NameFile;
-(NSArray *)listFiles;
-(void)deleteFile:(NSString *)_nameFile;
-(void)reloadTableView;
- (void)checkButtonTapped:(id)sender event:(id)event;
@end

@implementation SaveSimulationViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.

    self.listData = [self listFiles];

    [self.imputFileName becomeFirstResponder];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (IBAction)actionSave:(id)sender
{
    NSString * fileName = [self.imputFileName.text stringByTrimmingCharactersInSet: [NSCharacterSet whitespaceAndNewlineCharacterSet]];
    if ([fileName isEqualToString:@""]) {
        UIAlertView * alert = [[UIAlertView alloc]initWithTitle:@"Atenção"
                                                        message:@"Informe um nome para o arquivo."
                                                       delegate:self
                                              cancelButtonTitle:@"Ok"
                                              otherButtonTitles: nil];
        [alert show];
        return;
    }

    [self saveText: self.areaText.text nameFile: fileName];

    self.imputFileName.text = NULL;
    self.areaText.text = NULL;

    [self reloadTableView];
}

- (IBAction)actionClean:(id)sender
{
    self.imputFileName.text = NULL;
    self.areaText.text = NULL;

    //[self.imputFileName becomeFirstResponder];
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    NSString * nameFile = [self.listData objectAtIndex:indexPath.item];
    NSString * contentFile = [self displayContent:nameFile];

    self.imputFileName.text = nameFile;
    self.areaText.text = contentFile;
}

- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath;
{
    if (editingStyle == UITableViewCellEditingStyleDelete) {
        [self deleteFile: [self.listData objectAtIndex:indexPath.item]];
        [self reloadTableView];
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
    
    NSUInteger row = [indexPath row];
    cell.textLabel.text = [self.listData objectAtIndex:row];
    cell.imageView.image = [UIImage imageNamed:@"albert.icon.72x72.png"];
    cell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
    cell.selectionStyle = UITableViewCellSelectionStyleBlue;

	UIImage *image = [UIImage imageNamed:@"button_play.png"];

	UIButton *button = [UIButton buttonWithType:UIButtonTypeCustom];
	CGRect frame = CGRectMake(0.0, 0.0, 50, 50);
	button.frame = frame;

	[button setBackgroundImage:image forState:UIControlStateNormal];

	// set the button's target to this table view controller so we can interpret touch events and map that to a NSIndexSet
	[button addTarget:self action:@selector(checkButtonTapped:event:) forControlEvents:UIControlEventTouchUpInside];
	cell.backgroundColor = [UIColor clearColor];
	cell.accessoryView = button;

    return cell;
}

- (void)checkButtonTapped:(id)sender event:(id)event
{
	NSSet *touches = [event allTouches];
	UITouch *touch = [touches anyObject];
	CGPoint currentTouchPosition = [touch locationInView:self.tableView];
	NSIndexPath *indexPath = [self.tableView indexPathForRowAtPoint: currentTouchPosition];

	if (indexPath != nil){
        NSString * nameFile = [self.listData objectAtIndex:indexPath.item];
        NSString * nsContentFile = [self displayContent:nameFile];

        std::string stringContectFile = [nsContentFile cStringUsingEncoding: NSASCIIStringEncoding];

        Controller::getInstance()->clearSimularion();
        Controller::getInstance()->loadSceneFromFile(stringContectFile);
        self.tabBarController.selectedIndex = 1;
	}
}

-(void)saveText:(NSString *)_text nameFile:(NSString *) _nameFile
{
    // get the documents directory
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];

    // make a file name to write the data to using the documents directory:
    NSString *filePath = [NSString stringWithFormat:@"%@/%@", documentsDirectory, _nameFile];

    // save content to the documents directory
    [_text writeToFile:filePath
           atomically:NO
             encoding:NSStringEncodingConversionAllowLossy
                error:nil];

}

-(void)deleteFile:(NSString *)_nameFile
{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];

    NSString *filePath = [NSString stringWithFormat:@"%@/%@", documentsDirectory, _nameFile];

    [[NSFileManager defaultManager] removeItemAtPath:filePath error:nil];
}

-(NSArray *)listFiles
{
    // get the documents directory
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];

    NSArray *filePathsArray = [[NSFileManager defaultManager] subpathsOfDirectoryAtPath:documentsDirectory  error:nil];

    return filePathsArray;
}

-(NSString *)displayContent:(NSString *)_nameFile
{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];

    //make a file name to write the data to using the documents directory:
    NSString *filePath = [NSString stringWithFormat:@"%@/%@", documentsDirectory, _nameFile];
    NSString *content = [[NSString alloc] initWithContentsOfFile:filePath
                                                    usedEncoding:nil
                                                           error:nil];

    return content;
}

-(void)reloadTableView
{
    self.listData = [self listFiles];
    [_tableView reloadData];
}
@end
