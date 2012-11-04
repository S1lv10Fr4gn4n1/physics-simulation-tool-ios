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
    [saveController actionClean: nil];
    std::string strSimulation = Controller::getInstance()->generateSimulationToCharacter();

    if (strSimulation.size() <= 0) {
        return;
    }

    NSString * nsSimulation = [NSString stringWithCString:strSimulation.c_str() encoding:NSASCIIStringEncoding];
    saveController.areaText.text = nsSimulation;
}

- (IBAction)actionGenerateRandomSimulation:(id)sender
{
    UIAlertView * alert = [[UIAlertView alloc] initWithTitle:@"Simulação Aleatória"
                                                     message:@"Informe a quantidade de objetos"
                                                    delegate:self
                                           cancelButtonTitle:@"Criar objetos"
                                           otherButtonTitles:nil];
    alert.alertViewStyle = UIAlertViewStylePlainTextInput;
    UITextField * alertTextField = [alert textFieldAtIndex:0];
    alertTextField.keyboardType = UIKeyboardTypeNumberPad;
    alertTextField.returnKeyType = UIReturnKeyDone;
    alertTextField.placeholder = @"inteiro positivo";
    [alert show];
}

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
    NSString * strAux = [[alertView  textFieldAtIndex:0] text];

    bool onlyNumber = false;

    if (strAux != NULL && strAux.length > 0) {
        NSCharacterSet *alphaNums = [NSCharacterSet decimalDigitCharacterSet];
        NSCharacterSet *inStringSet = [NSCharacterSet characterSetWithCharactersInString:strAux];
        onlyNumber = [alphaNums isSupersetOfSet:inStringSet];
    }

    if (onlyNumber) {
        Controller::getInstance()->generateRandonSimulation(strAux.intValue);;
    } else {
        UIAlertView * alert = [[UIAlertView alloc] initWithTitle:@"Simulação Aleatória"
                                                         message:@"Valor informado não é um inteiro válido"
                                                        delegate:nil
                                               cancelButtonTitle:@"Ok"
                                               otherButtonTitles:nil];
        [alert show];
    }
}

@end
