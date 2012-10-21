//
//  ViewController.m
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani da Silva on 11/06/12.
//  
//

#import "SimulationViewController.h"

@interface SimulationViewController ()
- (void)setupGL;
- (void)tearDownGL;
- (void)initializeGestureRecognizer:(UIView *)view;

@property(nonatomic) BOOL changedPinch;
@property(nonatomic) BOOL changedRotate;

@end

static EAGLContext * context;

@implementation SimulationViewController

- (void)viewDidLoad
{
    [super viewDidLoad];

    if (!context) {
        context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
        
        if (!context) {
            NSLog(@"Failed to create ES context");
        }
    }
    
    GLKView *view = (GLKView *)self.view;
    view.context = context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    view.drawableMultisample = GLKViewDrawableMultisample4X;
    view.drawableColorFormat = GLKViewDrawableColorFormatRGB565;
    view.drawableStencilFormat = GLKViewDrawableStencilFormat8;

    self.preferredFramesPerSecond = FRAME_PER_SECOND;

    self.changedPinch = NO;
    self.changedRotate = NO;

    [self initializeGestureRecognizer: view];

    [self setupGL];

    if (Controller::getInstance()->isEditMode()) {
        self.buttonFind.hidden = false;
    } else {
        self.buttonFind.hidden = true;
    }

    [self animationLabelHelp];
}

- (void)viewDidUnload
{    
    [super viewDidUnload];
    
    [self tearDownGL];
    
    if ([EAGLContext currentContext] == context) {
        [EAGLContext setCurrentContext:nil];
    }
    
	context = nil;
}

- (void)loadView
{
    [super loadView];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    
    UIAlertView * alert = [[UIAlertView alloc]initWithTitle:@"Memory Warning" 
                                                    message:@"Memory Warning" 
                                                   delegate:self 
                                          cancelButtonTitle:@"Ok" 
                                          otherButtonTitles: nil];
    [alert show];
}

#ifdef __IPHONE_6_0
-(NSUInteger)supportedInterfaceOrientations
{
    if (self.view) {
        Controller::getInstance()->resizeScreen(self.view.bounds.size.width, self.view.bounds.size.height);
    }

	return UIInterfaceOrientationMaskLandscape;
}
#else
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    if (self.view) {
        Controller::getInstance()->resizeScreen(self.view.bounds.size.width, self.view.bounds.size.height);
    }

    return UIInterfaceOrientationIsLandscape(interfaceOrientation);
}
#endif

- (void)setupGL
{
    [EAGLContext setCurrentContext:context];
    
    if (!Controller::getInstance()->isInitialized()) {
        Controller::getInstance()->initializeEngine();
        Controller::getInstance()->initializeContextOpenGLES();
    }
}

- (void)tearDownGL
{
    [EAGLContext setCurrentContext:context];
    
    delete Controller::getInstance();
}

-(void)animationLabelHelp
{
    [self.labelHelp setAlpha:0.0];
    [UIView animateWithDuration:1.0
                          delay:0.5
                        options:UIViewAnimationOptionCurveLinear | UIViewAnimationOptionAllowUserInteraction
                     animations:^(void)
     {
         [self.labelHelp setAlpha:1.0];
     }
                     completion:^(BOOL finished)
     {
         if (finished) {
             [UIView animateWithDuration:1.0
                                   delay:1
                                 options:UIViewAnimationOptionCurveLinear | UIViewAnimationOptionAllowUserInteraction
                              animations:^(void)
              {
                  [self.labelHelp setAlpha:0.0];
              }
                              completion:^(BOOL finished)
              {
                  if (finished) {
                      self.labelHelp.hidden = YES;
                  }
              }];
         }
     }];
}

- (void)initializeGestureRecognizer:(UIView *)view
{
    UITapGestureRecognizer *doubleTapTwoFinger = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(doubleTapTwoFingerDetected:)];
    doubleTapTwoFinger.numberOfTapsRequired = 2;
    doubleTapTwoFinger.numberOfTouchesRequired = 2;
    [view addGestureRecognizer:doubleTapTwoFinger];

    UITapGestureRecognizer *oneTapThreeFinger = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(oneTapThreeFingerDetected:)];
    oneTapThreeFinger.numberOfTapsRequired = 1;
    oneTapThreeFinger.numberOfTouchesRequired = 3;
    [view addGestureRecognizer:oneTapThreeFinger];

//    UITapGestureRecognizer *doubleTapOneFinger = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(doubleTapOneFingerDetected:)];
//    doubleTapOneFinger.numberOfTapsRequired = 2;
//    doubleTapOneFinger.numberOfTouchesRequired = 1;
//    [view addGestureRecognizer:doubleTapOneFinger];

    UIPinchGestureRecognizer *pinchRecognizer = [[UIPinchGestureRecognizer alloc]initWithTarget:self action:@selector(pinchDetected:)];
    [view addGestureRecognizer:pinchRecognizer];
    
    UIRotationGestureRecognizer *rotationRecognizer = [[UIRotationGestureRecognizer alloc]initWithTarget:self action:@selector(rotationDetected:)];
    [view addGestureRecognizer:rotationRecognizer];
    
//    UILongPressGestureRecognizer *longPressRecognizer = [[UILongPressGestureRecognizer alloc]initWithTarget:self action:@selector(longPressDetected:)];
//    longPressRecognizer.minimumPressDuration = 2;
//    longPressRecognizer.numberOfTouchesRequired = 1;
//    [view addGestureRecognizer:longPressRecognizer];
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    CGPoint p = [[touches anyObject] locationInView: self.view];
    Controller::getInstance()->touchesBegan(p.x, p.y);
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    CGPoint p = [[touches anyObject] locationInView: self.view];
    Controller::getInstance()->touchesCancelled(p.x, p.y);
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    CGPoint p = [[touches anyObject] locationInView: self.view];
    Controller::getInstance()->touchesEnded(p.x, p.y);
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    CGPoint p = [[touches anyObject] locationInView: self.view];
    Controller::getInstance()->touchesMoved(p.x, p.y, [[touches allObjects] count]);
}

//- (IBAction)longPressDetected:(UIGestureRecognizer *)sender
//{
//    CGPoint p = [sender locationInView:sender.view];
//    Controller::getInstance()->longPressDetected(p.x, p.y);
//}

- (IBAction)oneTapThreeFingerDetected:(UIGestureRecognizer *)sender
{
    CGPoint p = [sender locationInView:sender.view];
    Controller::getInstance()->oneTapThreeFingerDetected(p.x, p.y);
}

//- (IBAction)swipeRightDetected:(UIGestureRecognizer *)sender
//{
//    CGPoint cgPoint = [sender locationInView:sender.view];
//    Controller::getInstance()->swipeRightDetected(MakePointer(cgPoint.x, cgPoint.y, 0));
//}
//
//- (IBAction)swipeLeftDetected:(UIGestureRecognizer *)sender
//{
//    CGPoint cgPoint = [sender locationInView:sender.view];
//    Controller::getInstance()->swipeLeftDetected(MakePointer(cgPoint.x, cgPoint.y, 0));
//}

- (IBAction)doubleTapTwoFingerDetected:(UIGestureRecognizer *)sender
{        
    Controller::getInstance()->stopSimulation();
    
    [self dismissViewControllerAnimated:YES completion:nil];
    [[UIApplication sharedApplication] setStatusBarHidden:NO withAnimation:UIStatusBarAnimationNone];
}

//- (IBAction)doubleTapOneFingerDetected:(UIGestureRecognizer *)sender
//{
//    CGPoint cgPoint = [sender locationInView:sender.view];    
//    Controller::getInstance()->doubleTapOneFingerDetected(cgPoint.x, cgPoint.y);
//}

- (IBAction)pinchDetected:(UIPinchGestureRecognizer *)pinchRecognizer
{
    if (self.changedRotate) {
        return;
    }

    if (pinchRecognizer.state == UIGestureRecognizerStateBegan ||
        pinchRecognizer.state == UIGestureRecognizerStateChanged) {
        self.changedPinch = YES;
    } else {
        self.changedPinch = NO;
    }

    if (pinchRecognizer.state == UIGestureRecognizerStateBegan) {
        Controller::getInstance()->pinchDetected(pinchRecognizer.scale, pinchRecognizer.velocity, true);
    }
    Controller::getInstance()->pinchDetected(pinchRecognizer.scale, pinchRecognizer.velocity, false);
}

- (IBAction)rotationDetected:(UIRotationGestureRecognizer *)rotationRecognizer
{
    if (self.changedPinch) {
        return;
    }

    if (rotationRecognizer.state == UIGestureRecognizerStateBegan ||
        rotationRecognizer.state == UIGestureRecognizerStateChanged) {
        self.changedRotate = YES;
    } else {
        self.changedRotate = NO;
    }

    if (rotationRecognizer.state == UIGestureRecognizerStateBegan) {
        Controller::getInstance()->rotationDetected(rotationRecognizer.rotation, rotationRecognizer.velocity, true);
    }
    Controller::getInstance()->rotationDetected(rotationRecognizer.rotation, rotationRecognizer.velocity, false);
}

#pragma mark - GLKView and GLKViewController delegate methods
- (void)update
{
    Controller::getInstance()->updateInformation(self.timeSinceLastDraw);
//    printf("frame: %u, fps: %u, lastDraw: %f\n", self.framesDisplayed, self.framesPerSecond, self.timeSinceLastDraw);
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    Controller::getInstance()->draw();
}

- (IBAction)actionFind:(id)sender {
}
@end
