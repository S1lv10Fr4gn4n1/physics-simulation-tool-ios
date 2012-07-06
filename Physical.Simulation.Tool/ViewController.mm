//
//  ViewController.m
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani da Silva on 11/06/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "ViewController.h"

@interface ViewController () {
    MainEngine * mainEngine;
    MainGraphic * mainGraphic;
    //EditorInforSimulation * editor;
    //ViewInforSimulation * infor;
}

@property (strong, nonatomic) EAGLContext *context;
@property (strong, nonatomic) GLKBaseEffect *effect;

- (void)setupGL;
- (void)tearDownGL;
- (void)initializeSimulator;
- (void)initializeFrame;
- (void)initializeLibraryGraphic;
- (void)initializeLibraryEngine;
@end


@implementation ViewController

@synthesize context = _context;
@synthesize effect = _effect;

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    
    if (!self.context) {
        NSLog(@"Failed to create ES context");
    }
    
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    
    [[UIApplication sharedApplication] setStatusBarHidden:YES withAnimation:UIStatusBarAnimationNone];
            
    [self setupGL];

    [self initializeSimulator];
}

- (void)viewDidUnload
{    
    [super viewDidUnload];
    
    [self tearDownGL];
    
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
	self.context = nil;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Release any cached data, images, etc. that aren't in use.
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    mainEngine->rotatedScreen(self.view.bounds.size.width, self.view.bounds.size.height);
    mainGraphic->rotatedScreen(self.view.bounds.size.width, self.view.bounds.size.height);
    
    return UIInterfaceOrientationIsLandscape(interfaceOrientation);
}

- (void)setupGL
{
    [EAGLContext setCurrentContext:self.context];
    
//    self.effect = [[GLKBaseEffect alloc] init];
//    self.effect.light0.enabled = GL_TRUE;
//    self.effect.light0.diffuseColor = GLKVector4Make(1.0f, 0.4f, 0.4f, 1.0f);
//    
//    glEnable(GL_DEPTH_TEST);
}

- (void)tearDownGL
{
    [EAGLContext setCurrentContext:self.context];
    
    self.effect = nil;
    
    delete mainGraphic;
    delete mainEngine;
}

- (void)initializeSimulator
{
    [self initializeLibraryGraphic];
    [self initializeLibraryEngine];
    [self initializeFrame];
}

- (void)initializeLibraryGraphic
{
    NSString *vertShaderPathname = [[NSBundle mainBundle] pathForResource:@"Shader" ofType:@"vsh"];
    NSString *fragShaderPathname = [[NSBundle mainBundle] pathForResource:@"Shader" ofType:@"fsh"];
    NSString *geomShaderPathname = [[NSBundle mainBundle] pathForResource:@"Shader" ofType:@"gsh"];
    const char* vertShaderSource = [[NSString stringWithContentsOfFile:vertShaderPathname encoding:NSUTF8StringEncoding error:nil] UTF8String];
    const char* fragShaderSource = [[NSString stringWithContentsOfFile:fragShaderPathname encoding:NSUTF8StringEncoding error:nil] UTF8String];
    const char* geomShaderSource = [[NSString stringWithContentsOfFile:geomShaderPathname encoding:NSUTF8StringEncoding error:nil] UTF8String];
    
    mainGraphic = new MainGraphic();
    mainGraphic->initializeShader(vertShaderSource, fragShaderSource, geomShaderSource);
    mainGraphic->initializeNDC(self.view.bounds.size.width, self.view.bounds.size.height);
}

- (void)initializeLibraryEngine
{
    mainEngine = new MainEngine();
    mainEngine->updateInformation();
    mainEngine->start();
}

- (void)initializeFrame
{
//    SimulatedObject * buttonStartEditor = new SimulatedObject();
//    buttonStartEditor->setMode(GL_TRIANGLE_FAN);
//    buttonStartEditor->setColor(MakeColor(0, 0, 0, 0, 4));
//    buttonStartEditor->setPhysicalFeature(MakePhysicalFeature(1, 1, 1, 1, 1));
//    buttonStartEditor->addPointer(MakePointer(-0.960938, -0.843750, 0.0));
//    buttonStartEditor->addPointer(MakePointer(-0.882812, -0.843750, 0.0));
//    buttonStartEditor->addPointer(MakePointer(-0.882812, -0.947917, 0.0));
//    buttonStartEditor->addPointer(MakePointer(-0.960938, -0.947917, 0.0));
//    
//    mainEngine->addSimulatedObjectInWorld(buttonStartEditor);
    
    //editor = [[EditorInforSimulation alloc]init];
    //infor = [[ViewInforSimulation alloc]init];
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    // TODO for implementing
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    // TODO for implementing
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
//    CGPoint cgPoint = [[touches anyObject] locationInView: self.view]; 
//    Pointer * pointer = MakePointer(cgPoint.x, cgPoint.y, 0);
//    mainGraphic->getNdc()->calcNDCCoordinates(&pointer->x, &pointer->y);
//
//    SimulatedObject * s = mainEngine->selectedSimulatedObject(pointer);
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    // TODO for implementing
}

#pragma mark - GLKView and GLKViewController delegate methods
- (void)update
{
    mainEngine->updateInformation();
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    mainGraphic->draw(mainEngine->getWorld());
}

@end
