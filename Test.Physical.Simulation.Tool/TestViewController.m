//
//  TestViewController.m
//  Test.Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 07/09/12.
//
//

#import "TestViewController.h"

@interface TestViewController ()
- (void)setupGL;
- (void)tearDownGL;
- (void)initializeGestureRecognizer:(UIView *)view;
@end

static EAGLContext * context;

@implementation TestViewController

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
    
    self.preferredFramesPerSecond = FRAME_PER_SECOND;
    
    [self initializeGestureRecognizer: view];
    
    [self setupGL];
    
    TestController::getInstance()->startSimulation();
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
                                                    message:@"Holy crap!! memory warning"
                                                   delegate:self
                                          cancelButtonTitle:@"Ok"
                                          otherButtonTitles: nil];
    [alert show];
}



#ifndef __IPHONE_6_0
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    if (self.view) {
        TestController::getInstance()->resizeScreen(self.view.bounds.size.width, self.view.bounds.size.height);
    }

    return UIInterfaceOrientationIsLandscape(interfaceOrientation);
}
#else
-(NSUInteger)supportedInterfaceOrientations
{
    if (self.view) {
        TestController::getInstance()->resizeScreen(self.view.bounds.size.width, self.view.bounds.size.height);
    }

	return UIInterfaceOrientationMaskLandscape;
}
#endif

- (void)setupGL
{
    [EAGLContext setCurrentContext:context];
    
    if (!TestController::getInstance()->isInitialized()) {
        TestController::getInstance()->initializeEngine();
        TestController::getInstance()->initializeContextOpenGLES();
    }
}

- (void)tearDownGL
{
    [EAGLContext setCurrentContext:context];
    
    TestController::getInstance()->freeObjects();
}

- (void)initializeGestureRecognizer:(UIView *)view
{
//    UITapGestureRecognizer *doubleTapTwoFinger = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(doubleTapTwoFingerDetected:)];
//    doubleTapTwoFinger.numberOfTapsRequired = 2;
//    doubleTapTwoFinger.numberOfTouchesRequired = 2;
//    [view addGestureRecognizer:doubleTapTwoFinger];
    
    UITapGestureRecognizer *oneTapThreeFinger = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(oneTapThreeFingerDetected:)];
    oneTapThreeFinger.numberOfTapsRequired = 1;
    oneTapThreeFinger.numberOfTouchesRequired = 3;
    [view addGestureRecognizer:oneTapThreeFinger];
    
    UITapGestureRecognizer *doubleTapOneFinger = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(doubleTapOneFingerDetected:)];
    doubleTapOneFinger.numberOfTapsRequired = 2;
    doubleTapOneFinger.numberOfTouchesRequired = 1;
    [view addGestureRecognizer:doubleTapOneFinger];
    
    UIPinchGestureRecognizer *pinchRecognizer = [[UIPinchGestureRecognizer alloc]initWithTarget:self action:@selector(pinchDetected:)];
    [view addGestureRecognizer:pinchRecognizer];
    
    UIRotationGestureRecognizer *rotationRecognizer = [[UIRotationGestureRecognizer alloc]initWithTarget:self action:@selector(rotationDetected:)];
    [view addGestureRecognizer:rotationRecognizer];
    
    UILongPressGestureRecognizer *longPressRecognizer = [[UILongPressGestureRecognizer alloc]initWithTarget:self action:@selector(longPressDetected:)];
    longPressRecognizer.minimumPressDuration = 2;
    longPressRecognizer.numberOfTouchesRequired = 1;
    [view addGestureRecognizer:longPressRecognizer];
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    CGPoint p = [[touches anyObject] locationInView: self.view];
    TestController::getInstance()->touchesBegan(p.x, p.y);
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    CGPoint p = [[touches anyObject] locationInView: self.view];
    TestController::getInstance()->touchesCancelled(p.x, p.y);
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    CGPoint p = [[touches anyObject] locationInView: self.view];
    TestController::getInstance()->touchesEnded(p.x, p.y);
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    CGPoint p = [[touches anyObject] locationInView: self.view];
    TestController::getInstance()->touchesMoved(p.x, p.y, [[touches allObjects] count]);
}

- (IBAction)longPressDetected:(UIGestureRecognizer *)sender
{
    CGPoint p = [sender locationInView:sender.view];
    TestController::getInstance()->longPressDetected(p.x, p.y);
}

- (IBAction)oneTapThreeFingerDetected:(UIGestureRecognizer *)sender
{
    CGPoint p = [sender locationInView:sender.view];
    TestController::getInstance()->oneTapThreeFingerDetected(p.x, p.y);
}

//- (IBAction)swipeRightDetected:(UIGestureRecognizer *)sender
//{
//    CGPoint cgPoint = [sender locationInView:sender.view];
//    TestController::getInstance()->swipeRightDetected(MakePointer(cgPoint.x, cgPoint.y, 0));
//}
//
//- (IBAction)swipeLeftDetected:(UIGestureRecognizer *)sender
//{
//    CGPoint cgPoint = [sender locationInView:sender.view];
//    TestController::getInstance()->swipeLeftDetected(MakePointer(cgPoint.x, cgPoint.y, 0));
//}

- (IBAction)doubleTapTwoFingerDetected:(UIGestureRecognizer *)sender
{
    TestController::getInstance()->stopSimulation();
    
    [self dismissViewControllerAnimated:YES completion:nil];
    [[UIApplication sharedApplication] setStatusBarHidden:NO withAnimation:UIStatusBarAnimationNone];
}

- (IBAction)doubleTapOneFingerDetected:(UIGestureRecognizer *)sender
{
    CGPoint cgPoint = [sender locationInView:sender.view];
    TestController::getInstance()->doubleTapOneFingerDetected(cgPoint.x, cgPoint.y);
}

- (IBAction)pinchDetected:(UIGestureRecognizer *)sender
{
    CGFloat scale = [(UIPinchGestureRecognizer *)sender scale];
    CGFloat velocity = [(UIPinchGestureRecognizer *)sender velocity];
    
    if (sender.state == UIGestureRecognizerStateBegan) {
        TestController::getInstance()->pinchDetected(scale, velocity, true);
    }
    TestController::getInstance()->pinchDetected(scale, velocity, false);
}

- (IBAction)rotationDetected:(UIGestureRecognizer *)sender
{
    CGFloat radians = [(UIRotationGestureRecognizer *)sender rotation];
    CGFloat velocity = [(UIRotationGestureRecognizer *)sender velocity];
    
    if (sender.state == UIGestureRecognizerStateBegan) {
        TestController::getInstance()->rotationDetected(radians, velocity, true);
    }
    TestController::getInstance()->rotationDetected(radians, velocity, false);
}

#pragma mark - GLKView and GLKViewController delegate methods
- (void)update
{
    TestController::getInstance()->updateInformation(self.timeSinceLastDraw);
    self.labelFPS.text = [[NSString alloc]initWithFormat:@"%u fps", self.framesPerSecond];
//    NSLog(@"frame: %u, fps: %u, lastDraw: %f", self.framesDisplayed, self.framesPerSecond, self.timeSinceLastDraw);
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    TestController::getInstance()->draw();
}

- (IBAction)actionRemake:(id)sender
{
    TestController::getInstance()->stopSimulation();
    TestController::getInstance()->clearSimularion();
    TestController::getInstance()->startSimulation();
}
@end
